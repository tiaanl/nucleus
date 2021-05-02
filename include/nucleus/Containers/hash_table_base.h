#pragma once

#include <cstring>

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/hash.h"

namespace nu {

template <typename ItemType>
struct DefaultHashTableBaseTraits {
  static HashedValue hashed(const ItemType& item) {
    return Hash<ItemType>::hashed(item);
  }

  static bool equals(const ItemType& left, const ItemType& right) {
    return left == right;
  }
};

template <typename ItemType, typename Traits = DefaultHashTableBaseTraits<ItemType>>
class HashTableBase {
public:
  class Iterator {
  public:
    ItemType& operator*() const {
      return hash_table_->buckets_[index_].reference();
    }

    ItemType* operator->() {
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
    friend class HashTableBase<ItemType, Traits>;

    Iterator(const HashTableBase<ItemType, Traits>* hash_table, MemSize index)
      : hash_table_{hash_table}, index_{index} {}

    const HashTableBase<ItemType, Traits>* hash_table_;
    MemSize index_;
  };

  HashTableBase() = default;

  ~HashTableBase() {
    for (MemSize i = 0; i < size_; ++i) {
      buckets_[i].reference().~ItemType();
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

protected:
  struct Bucket {
    bool is_used() const {
      return used_ == 1;
    }

    bool is_deleted() const {
      return deleted_;
    }

    void set(ItemType&& item) {
      new (data_) ItemType{std::forward<ItemType>(item)};
      used_ = 1;
      deleted_ = 0;
    }

    void clear() {
      reference().~ItemType();
      used_ = 0;
      deleted_ = 1;
    }

    ItemType* pointer() {
      return reinterpret_cast<ItemType*>(&data_);
    }

    ItemType& reference() {
      return reinterpret_cast<ItemType&>(*reinterpret_cast<ItemType*>(&data_));
    }

  private:
    U8 data_[sizeof(ItemType)];
    U8 used_;
    U8 deleted_;
  };

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

  static_assert(std::is_trivially_constructible_v<Bucket>);

  static constexpr MemSize MIN_SIZE = 4;

  template <typename Predicate>
  Bucket* find_bucket_for_reading(HashedValue hash, Predicate predicate) const {
    if (capacity_ == 0) {
      return nullptr;
    }

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
          auto old_hash = Traits::hashed(old_buckets[i].reference());
          Bucket* new_bucket = find_bucket_for_writing(old_hash, [&](ItemType& t) {
            return Traits::equals(t, old_buckets[i].reference());
          });
          new_bucket->set(std::move(old_buckets[i].reference()));

          old_buckets[i].pointer()->~ItemType();
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

  MemSize size_ = 0;
  MemSize capacity_ = 0;
  Bucket* buckets_ = nullptr;
};

}  // namespace nu
