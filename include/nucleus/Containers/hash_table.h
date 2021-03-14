#pragma once

#include <cstring>

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Types.h"
#include "nucleus/hash.h"

namespace nu {

template <typename HashTableType, typename T, typename BucketType>
class HashTableIterator {
public:
  bool operator==(const HashTableIterator& other) const {
    return bucket_ == other.bucket_;
  }

  bool operator!=(const HashTableIterator& other) const {
    return bucket_ != other.bucket_;
  }

  T& operator*() {
    return *bucket_->slot();
  }

  T* operator->() {
    return bucket_->slot();
  }

  void operator++() {
    next();
  }

private:
  friend HashTableType;

  void next() {
    if (!bucket_) {
      return;
    }

    do {
      ++bucket_;
      if (bucket_->used) {
        return;
      }
    } while (!bucket_->end);

    if (bucket_->end) {
      bucket_ = nullptr;
    }
  }

  explicit HashTableIterator(BucketType* bucket) : bucket_{bucket} {}

  BucketType* bucket_ = nullptr;
};

template <typename T>
class HashTable {
private:
  struct Bucket {
    bool used;
    bool deleted;
    bool end;
    alignas(T) U8 storage[sizeof(T)];

    T* slot() {
      return (T*)storage;
    }
  };

public:
  static constexpr MemSize k_min_capacity = 4;

  using Iterator = HashTableIterator<HashTable, T, Bucket>;
  using ConstIterator = HashTableIterator<const HashTable, const T, const Bucket>;

  HashTable() = default;

  explicit HashTable(MemSize initial_capacity) {
    rehash(initial_capacity);
  }

  HashTable(const HashTable& other) {
    rehash(other.capacity_);
    for (auto& it : other) {
      set(it);
    }
  }

  HashTable(HashTable&& other) noexcept
    : buckets_(other.buckets_),
      size_(other.size_),
      capacity_(other.capacity_),
      deleted_count_(other.deleted_count_) {
    other.buckets_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    other.deleted_count_ = 0;
  }

  ~HashTable() {
    if (!buckets_) {
      return;
    }

    for (MemSize i = 0; i < capacity_; ++i) {
      if (buckets_[i].used) {
        buckets_[i].slot()->~T();
      }
    }

    std::free(buckets_);
  }

  HashTable& operator=(const HashTable& other) {
    HashTable temp{other};
    swap(*this, temp);

    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept {
    swap(*this, other);

    return *this;
  }

  NU_NO_DISCARD bool empty() const {
    return !size_;
  }

  NU_NO_DISCARD MemSize size() const {
    return size_;
  }

  NU_NO_DISCARD MemSize capacity() const {
    return capacity_;
  }

  Iterator begin() {
    for (MemSize i = 0; i < capacity_; ++i) {
      if (buckets_[i].used) return Iterator{&buckets_[i]};
    }
    return end();
  }

  Iterator end() {
    return Iterator{nullptr};
  }

  ConstIterator begin() const {
    for (MemSize i = 0; i < capacity_; ++i) {
      if (buckets_[i].used) {
        return ConstIterator{&buckets_[i]};
      }
    }

    return end();
  }

  ConstIterator end() const {
    return ConstIterator{nullptr};
  }

  friend void swap(HashTable& left, HashTable& right) noexcept {
    using std::swap;

    swap(left.buckets_, right.buckets_);
    swap(left.size_, right.size_);
    swap(left.capacity_, right.capacity_);
    swap(left.deleted_count_, right.deleted_count_);
  }

  // Returns true if a new item was inserted.
  template <typename U = T>
  bool set(U&& item) {
    auto& bucket = lookup_for_writing(item);
    if (bucket.used) {
      (*bucket.slot()) = std::forward<U>(item);
      return false;
    }

    new (bucket.slot()) T(std::forward<U>(item));
    bucket.used = true;
    if (bucket.deleted) {
      bucket.deleted = false;
      --deleted_count_;
    }
    ++size_;
    return true;
  }

  template <typename Finder>
  Iterator find(HashedValue hash, Finder finder) {
    return Iterator{lookup_with_hash(hash, std::move(finder))};
  }

  Iterator find(const T& item) {
    auto hash = Hash<T>::hashed(item);
    return find(hash, [&](auto& other) {
      return item == other;
    });
  }

  template <typename Finder>
  ConstIterator find(HashedValue hash, Finder finder) const {
    return ConstIterator{lookup_with_hash(hash, std::move(finder))};
  }

  ConstIterator find(const T& item) const {
    auto hash = Hash<T>::hashed(item);
    return find(hash, [&](auto& other) {
      return item == other;
    });
  }

  bool contains(const T& item) const {
    return find(item) != end();
  }

  // Returns true is the item was deleted.
  bool remove(const T& item) {
    auto it = find(item);
    if (it != end()) {
      remove(it);
      return true;
    }

    return false;
  }

  void remove(Iterator iterator) {
    DCHECK(iterator.bucket_);

    auto& bucket = *iterator.bucket_;
    DCHECK(bucket.used);
    DCHECK(!bucket.end);
    DCHECK(!bucket.deleted);

    bucket.slot()->~T();
    bucket.used = false;
    bucket.deleted = true;

    --size_;
    ++deleted_count_;
  }

private:
  NU_NO_DISCARD MemSize used_bucket_count() const {
    return size_ + deleted_count_;
  }

  NU_NO_DISCARD bool should_grow() const {
    return ((used_bucket_count() + 1) * 100) >= (capacity_ * 60);
  }

  template <typename Finder>
  Bucket* lookup_with_hash(unsigned hash, Finder finder,
                           Bucket** usable_bucket_for_writing = nullptr) const {
    if (size_ == 0) {
      return nullptr;
    }

    MemSize bucket_index = hash % capacity_;
    for (;;) {
      auto& bucket = buckets_[bucket_index];

      if (usable_bucket_for_writing && !*usable_bucket_for_writing && !bucket.used) {
        *usable_bucket_for_writing = &bucket;
      }

      if (bucket.used && finder(*bucket.slot())) {
        return &bucket;
      }

      if (!bucket.used && !bucket.deleted) {
        return nullptr;
      }

      hash = double_hash(hash);
      bucket_index = hash % capacity_;
    }
  }

  Bucket& lookup_for_writing(const T& item) {
    auto hash = Hash<T>::hashed(item);
    Bucket* usable_bucket_for_writing = nullptr;

    auto finder = [&item](auto& entry) {
      return entry == item;
    };

    Bucket* bucket_for_reading = lookup_with_hash(hash, finder, &usable_bucket_for_writing);
    if (bucket_for_reading) {
      return *const_cast<Bucket*>(bucket_for_reading);
    }

    if (should_grow()) {
      rehash(capacity_ * 2);
    } else if (usable_bucket_for_writing) {
      return *usable_bucket_for_writing;
    }

    MemSize bucket_index = hash % capacity_;

    for (;;) {
      auto& bucket = buckets_[bucket_index];
      if (!bucket.used) {
        return bucket;
      }
      hash = double_hash(hash);
      bucket_index = hash % capacity_;
    }
  }

  void insert_during_rehash(T&& item) {
    Bucket& bucket = lookup_for_writing(item);
    new (bucket.slot()) T(std::move(item));
    bucket.used = true;
  }

  void rehash(MemSize new_capacity) {
    new_capacity = std::max(new_capacity, k_min_capacity);

    Bucket* old_buckets = buckets_;
    MemSize old_capacity = capacity_;

    buckets_ = static_cast<Bucket*>(std::malloc(sizeof(Bucket) * (new_capacity + 1)));
    std::memset(buckets_, 0, sizeof(Bucket) * (new_capacity + 1));
    capacity_ = new_capacity;
    deleted_count_ = 0;

    buckets_[capacity_].end = true;

    if (!old_buckets) {
      return;
    }

    for (MemSize i = 0; i < old_capacity; ++i) {
      Bucket& old_bucket = old_buckets[i];
      if (old_bucket.used) {
        insert_during_rehash(std::move(*old_bucket.slot()));
        old_bucket.slot()->~T();
      }
    }

    std::free(old_buckets);
  }

  Bucket* buckets_ = nullptr;
  MemSize size_ = 0;
  MemSize capacity_ = 0;
  MemSize deleted_count_ = 0;
};

}  // namespace nu
