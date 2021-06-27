#pragma once

#include <cstring>

#include "nucleus/Containers/hash_table_base.h"
#include "nucleus/hash.h"
#include "nucleus/logging.h"
#include "nucleus/macros.h"
#include "nucleus/types.h"

namespace nu {

// Implementation of a hash table using linear probing for resolving collisions.
template <typename T>
class HashTable : public HashTableBase<T> {
public:
  HashTable() = default;

  bool contains(const T& item) const {
    auto hash = Hash<T>::hashed(item);
    return this->find_bucket_for_reading(hash, [&](T& t) {
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
    auto* bucket = this->find_bucket_for_writing(hash, [&](T& t) {
      return item == t;
    });
    DCHECK(bucket) << "Could not find a bucket for writing.";

    bool is_new = !bucket->is_used();

    bucket->set(std::forward<T>(item));

    if (is_new) {
      ++this->size_;
    }

    return {is_new, bucket->pointer()};
  }

  // Returns true if the item was found and removed from the table.
  bool remove(const T& item) {
    auto hash = Hash<T>::hashed(item);
    auto* bucket = this->find_bucket_for_reading(hash, [&](T& t) {
      return item == t;
    });
    if (!bucket) {
      return false;
    }

    bucket->clear();

    --this->size_;

    return true;
  }
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
