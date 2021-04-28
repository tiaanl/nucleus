#pragma once

#include <cstring>

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Types.h"
#include "nucleus/hash.h"

namespace nu {

template <typename T>
class HashTable {
public:
  class Iterator {
  public:
    T& operator*() const {
      return *hash_table_->buckets_[index_].typed();
    }

    T* operator->() {
      return hash_table_->buckets_[index_].typed();
    }

    Iterator& operator++() {
      index_++;
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

  HashTable() : size_{0}, capacity_{0}, buckets_{nullptr} {}

  ~HashTable() {
    for (MemSize i = 0; i < size_; ++i) {
      buckets_[i].typed()->~T();
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

  Iterator begin() {
    return Iterator{this, 0};
  }

  Iterator end() {
    return Iterator{this, size_};
  }

  bool contains(const T& item) const {
    auto hash = Hash<T>::hashed(item);
    MemSize index = hash % capacity_;

    MemSize find_count = 0;
    while (!buckets_[index].used && find_count < size_) {
      hash = double_hash(hash);
      index = hash % capacity_;
    }

    return *buckets_[index].typed() == item;
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
    if (capacity_ > 0) {
      auto hash = Hash<T>::hashed(item);
      MemSize index = hash % capacity_;

      while (!buckets_[index].used) {
        hash = double_hash(hash);
        index = hash % capacity_;
      }
      DCHECK(!buckets_[index].used) << "Found an already used bucket.";

      if (*buckets_[index].typed() == item) {
        new (buckets_[index].typed()) T{std::forward<T>(item)};
        return {false, buckets_[index].typed()};
      }
    }

    ensure_capacity(size_ + 1);

    auto hash = Hash<T>::hashed(item);
    MemSize index = hash % capacity_;

    while (buckets_[index].used && *buckets_[index].typed() != item) {
      hash = double_hash(hash);
      index = hash % capacity_;
    }
    DCHECK(!buckets_[index].used) << "Found an already used bucket.";

    // Attempt to call the move constructor on the storage.
    new (buckets_[index].typed()) T{std::forward<T>(item)};
    buckets_[index].used = 1;

    size_++;

    return {true, buckets_[index].typed()};
  }

  bool remove(const T& NU_UNUSED(item)) {
    return false;
  }

private:
  // Make sure we have enough capacity to accommodate `new_capacity` items.  Grow if needed, but
  // never shrinks.
  void ensure_capacity(MemSize new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }

    auto* old_buckets = buckets_;

    buckets_ = static_cast<Bucket*>(std::malloc(sizeof(Bucket) * new_capacity));
    // TODO: Should we only clear out the used and next values?
    std::memset(buckets_, 0, sizeof(Bucket) * new_capacity);
    capacity_ = new_capacity;

    if (old_buckets) {
      for (MemSize i = 0; i < size_; ++i) {
        auto hash = Hash<T>::hashed(*old_buckets[i].typed());
        MemSize index = hash % capacity_;

        while (buckets_[index].used) {
          hash = double_hash(hash);
          index = hash % capacity_;
        }

        DCHECK(!buckets_[index].used) << "Found an already used bucket.";

        // Attempt to call the move constructor on the storage.
        new (buckets_[index].typed()) T{std::move(*old_buckets[i].typed())};
        buckets_[index].used = 1;
      }

      std::free(old_buckets);
    }
  }

  struct Bucket {
    U8 data[sizeof(T)];
    U8 used;

    T* typed() {
      return reinterpret_cast<T*>(&data);
    }
  };

  MemSize size_;
  MemSize capacity_;
  Bucket* buckets_;
};

}  // namespace nu
