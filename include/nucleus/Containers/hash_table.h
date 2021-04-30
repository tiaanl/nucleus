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
      buckets_[i].pointer()->~T();
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

    Iterator(HashTable<T>* hash_table, MemSize index) : hash_table_{hash_table}, index_{index} {}

    HashTable<T>* hash_table_;
    MemSize index_;
  };

  Iterator begin() {
    return Iterator{this, index_of_first_used_bucket_from(0)};
  }

  Iterator end() {
    return Iterator{this, capacity_};
  }

  bool contains(const T& item) const {
    return find_bucket_for_reading(item) != nullptr;
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
    auto* bucket = find_bucket_for_writing(item);
    DCHECK(bucket) << "Could not find a bucket for writing.";

    bool is_new = !bucket->used;

    new (bucket->pointer()) T{std::forward<T>(item)};
    bucket->used = 1;

    ++size_;

    return {is_new, bucket->pointer()};
  }

  bool remove(const T& NU_UNUSED(item)) {
    return false;
  }

private:
  struct Bucket {
    U8 data[sizeof(T)];
    U8 used;

    T* pointer() {
      return reinterpret_cast<T*>(&data);
    }

    T& reference() {
      return reinterpret_cast<T&>(*reinterpret_cast<T*>(&data));
    }
  };

  static_assert(std::is_trivially_constructible_v<Bucket>);

  static constexpr MemSize MIN_SIZE = 4;

  Bucket* find_bucket_for_reading(const T& item) const {
    auto hash = Hash<T>::hashed(item);
    MemSize index = hash % capacity_;
    MemSize start_index = index;

    for (;;) {
      Bucket* bucket = &buckets_[index];

      if (!bucket->used) {
        return nullptr;
      }

      if (bucket->reference() == item) {
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
        if (old_buckets[i].used) {
          Bucket* new_bucket = find_bucket_for_writing(old_buckets[i].reference());
          new (new_bucket->pointer()) T(std::move(old_buckets[i].reference()));
          new_bucket->used = 1;

          old_buckets[i].pointer()->~T();
        }
      }
    }
  }

  Bucket* find_bucket_for_writing(const T& item) {
    ensure_capacity(size_ + 1);

    auto hash = Hash<T>::hashed(item);
    MemSize index = hash % capacity_;
    MemSize start_index = index;

    for (;;) {
      Bucket* bucket = &buckets_[index];

      if (bucket->used && bucket->reference() == item) {
        return bucket;
      }

      if (!bucket->used) {
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

  MemSize index_of_first_used_bucket_from(MemSize start) {
    for (;;) {
      if (start == capacity_) {
        break;
      }

      if (buckets_[start].used) {
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

}  // namespace nu
