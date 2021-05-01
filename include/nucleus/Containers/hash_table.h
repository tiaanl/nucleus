#pragma once

#include <cstring>

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Types.h"
#include "nucleus/hash.h"

namespace nu {

// Implementation of a hash table using linear probing for resolving collisions.
template <typename T>
class HashTable {
public:
  HashTable() : size_{0}, capacity_{0}, buckets_{nullptr} {}

  ~HashTable() {
    for (MemSize i = 0; i < size_; ++i) {
      buckets_[i].reference().~T();
    }
    std::free(buckets_);
  }

  NU_NO_DISCARD MemSize size() const {
    return size_;
  }

  NU_NO_DISCARD bool empty() const {
    return size_ == 0;
  }

  NU_NO_DISCARD MemSize capacity() const {
    return capacity_;
  }

  class Iterator {
  public:
    T& operator*() const {
      return hash_table_->buckets_[index_].reference();
    }

    T* operator->() {
      return hash_table_->buckets_[index_].pointer();
    }

    Iterator& operator++() {
      index_ = hash_table_->index_of_first_used_bucket_from(index_ + 1);

      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& left, const Iterator& right) {
      return left.hash_table_ == right.hash_table_ && left.index_ == right.index_;
    }

    friend bool operator!=(const Iterator& left, const Iterator& right) {
      return left.hash_table_ != right.hash_table_ || left.index_ != right.index_;
    }

  private:
    friend class HashTable<T>;

    Iterator(const HashTable<T>* hash_table, MemSize index)
      : hash_table_{hash_table}, index_{index} {}

    const HashTable<T>* hash_table_;
    MemSize index_;
  };

  Iterator begin() const {
    return Iterator{this, index_of_first_used_bucket_from(0)};
  }

  Iterator begin() {
    return Iterator{this, index_of_first_used_bucket_from(0)};
  }

  Iterator end() const {
    return Iterator{this, capacity_};
  }

  Iterator end() {
    return Iterator{this, capacity_};
  }

  bool contains(const T& item) const {
    auto hash = Hash<T>::hashed(item);
    return find_bucket_for_reading(hash, [&](T& t) {
             return item == t;
           }) != nullptr;
  }

  class FindResult {
  public:
    bool was_found() const {
      return found_;
    }

    T& item() {
      DCHECK(item_ != nullptr);
      return *item_;
    }

  private:
    friend class HashTable<T>;

    FindResult(bool found, T* item) : found_{found}, item_{item} {}

    bool found_;
    T* item_;
  };

  FindResult find(const T& item) {
    auto* bucket = find_bucket_for_reading(Hash<T>::hashed(item), [&](T& t) {
      return t == item;
    });
    if (bucket) {
      return {true, bucket->pointer()};
    }

    return {false, nullptr};
  }

  template <typename Predicate>
  FindResult find(HashedValue hash, Predicate predicate) {
    auto* bucket = find_bucket_for_reading(hash, predicate);
    if (bucket) {
      return {true, bucket->pointer()};
    }

    return {false, nullptr};
  }

  class InsertResult {
  public:
    NU_NO_DISCARD bool is_new() const {
      return is_new_;
    }

    NU_NO_DISCARD T& item() {
      return *item_;
    }

  private:
    friend class HashTable<T>;

    InsertResult(bool is_new, T* item) : is_new_{is_new}, item_{item} {}

    bool is_new_;
    T* item_;
  };

  InsertResult insert(T&& item) {
    auto hash = Hash<T>::hashed(item);
    auto* bucket = find_bucket_for_writing(hash, [&](T& t) {
      return item == t;
    });
    DCHECK(bucket) << "Could not find a bucket for writing.";

    bool is_new = !bucket->is_used();

    bucket->set(std::forward<T>(item));

    if (is_new) {
      ++size_;
    }

    return {is_new, bucket->pointer()};
  }

  // Returns true if the item was found and removed from the table.
  bool remove(const T& item) {
    auto hash = Hash<T>::hashed(item);
    auto* bucket = find_bucket_for_reading(hash, [&](T& t) {
      return item == t;
    });
    if (!bucket) {
      return false;
    }

    bucket->clear();

    --size_;

    return true;
  }

private:
  struct Bucket {
    bool is_used() const {
      return used_ == 1;
    }

    bool is_deleted() const {
      return deleted_;
    }

    void set(T&& item) {
      new (data_) T{std::forward<T>(item)};
      used_ = 1;
      deleted_ = 0;
    }

    void clear() {
      reference().~T();
      used_ = 0;
      deleted_ = 1;
    }

    T* pointer() {
      return reinterpret_cast<T*>(&data_);
    }

    T& reference() {
      return reinterpret_cast<T&>(*reinterpret_cast<T*>(&data_));
    }

  private:
    U8 data_[sizeof(T)];
    U8 used_;
    U8 deleted_;
  };

  static_assert(std::is_trivially_constructible_v<Bucket>);

  static constexpr MemSize MIN_SIZE = 4;

  template <typename Predicate>
  Bucket* find_bucket_for_reading(HashedValue hash, Predicate predicate) const {
    MemSize index = hash % capacity_;
    MemSize start_index = index;

    for (;;) {
      Bucket* bucket = &buckets_[index];

      if (!bucket->is_used() && !bucket->is_deleted()) {
        return nullptr;
      }

      if (!bucket->is_deleted() && predicate(bucket->reference())) {
        return bucket;
      }

      ++index;
      index %= capacity_;

      if (index == start_index) {
        break;
      }
    }

    return nullptr;
  }

  void ensure_capacity(MemSize required_capacity) {
    if (required_capacity <= capacity_) {
      return;
    }

    MemSize new_capacity = std::max(capacity_ * 2, MIN_SIZE);
    DCHECK(required_capacity <= new_capacity);

    Bucket* old_buckets = buckets_;
    MemSize old_capacity = capacity_;

    buckets_ = static_cast<Bucket*>(std::malloc(sizeof(Bucket) * new_capacity));
    std::memset(buckets_, 0, sizeof(Bucket) * new_capacity);
    capacity_ = new_capacity;

    if (old_buckets) {
      for (MemSize i = 0; i < old_capacity; ++i) {
        if (old_buckets[i].is_used()) {
          auto old_hash = Hash<T>::hashed(old_buckets[i].reference());
          Bucket* new_bucket = find_bucket_for_writing(old_hash, [&](T& t) {
            return t == old_buckets[i].reference();
          });
          new_bucket->set(std::move(old_buckets[i].reference()));

          old_buckets[i].pointer()->~T();
        }
      }

      std::free(old_buckets);
    }
  }

  template <typename Predicate>
  Bucket* find_bucket_for_writing(HashedValue hash, Predicate predicate) {
    ensure_capacity(size_ + 1);

    MemSize index = hash % capacity_;
    MemSize start_index = index;

    for (;;) {
      Bucket* bucket = &buckets_[index];

      if (bucket->is_used() && predicate(bucket->reference())) {
        return bucket;
      }

      if (!bucket->is_used()) {
        return bucket;
      }

      ++index;
      index %= capacity_;

      if (index == start_index) {
        break;
      }
    }

    return nullptr;
  }

  NU_NO_DISCARD MemSize index_of_first_used_bucket_from(MemSize start) const {
    for (;;) {
      if (start == capacity_) {
        break;
      }

      if (buckets_[start].is_used()) {
        break;
      }

      ++start;
    }

    return start;
  }

  MemSize size_;
  MemSize capacity_;
  Bucket* buckets_;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const HashTable<T>& hash_table) {
  os << '[';
  MemSize i = 1;
  for (const auto& item : hash_table) {
    os << item;
    if (i++ < hash_table.size()) {
      os << ", ";
    }
  }
  os << ']';
  return os;
}

}  // namespace nu
