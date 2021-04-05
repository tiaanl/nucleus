#pragma once

#include "nucleus/Containers/hash_table.h"

namespace nu {

template <typename KeyType, typename ValueType>
struct HashMapItem {
  KeyType key;
  ValueType value;
};

template <typename KeyType, typename ValueType>
struct Hash<HashMapItem<KeyType, ValueType>> {
  static HashedValue hashed(const auto& value) {
    return Hash<KeyType>::hashed(value.key);
  }
};

template <typename KeyType, typename ValueType>
inline bool operator==(const HashMapItem<KeyType, ValueType>& left,
                       const HashMapItem<KeyType, ValueType>& right) {
  return left.key == right.key;
}

template <typename KeyType, typename ValueType>
class HashMap {
public:
  using Iterator = typename HashTable<HashMapItem<KeyType, ValueType>>::Iterator;
  using ConstIterator = typename HashTable<HashMapItem<KeyType, ValueType>>::ConstIterator;

  HashMap() = default;

  NU_NO_DISCARD bool empty() const {
    return items_.empty();
  }

  NU_NO_DISCARD MemSize size() const {
    return items_.size();
  }

  NU_NO_DISCARD MemSize capacity() const {
    return items_.capacity();
  }

  class SetResult {
  public:
    NU_NO_DISCARD bool is_new() const {
      return is_new_;
    }

    KeyType& key() {
      return *key_;
    }

    ValueType& value() {
      return *value_;
    }

  private:
    friend HashMap;

    SetResult(bool is_new, KeyType* key, ValueType* value)
      : is_new_{is_new}, key_{key}, value_{value} {}

    bool is_new_;
    KeyType* key_;
    ValueType* value_;
  };

  SetResult set(const KeyType& key, ValueType value) {
    auto result = items_.set({std::move(key), std::move(value)});

    return {result.is_new(), &result.item().key, &result.item().value};
  }

  bool contains_key(const KeyType& key) const {
    return find(key) != end();
  }

  Iterator begin() {
    return items_.begin();
  }

  Iterator end() {
    return items_.end();
  }

  Iterator find(const KeyType& key) {
    auto hash = Hash<KeyType>::hashed(key);
    return items_.find(hash, [&](const auto& entry) {
      return key == entry.key;
    });
  }

  template <typename Finder>
  Iterator find(unsigned hash, Finder finder) {
    return items_.find(hash, finder);
  }

  ConstIterator begin() const {
    return items_.begin();
  }

  ConstIterator end() const {
    return items_.end();
  }

  ConstIterator find(const KeyType& key) const {
    auto hash = Hash<KeyType>::hashed(key);
    return items_.find(hash, [&](auto& entry) {
      return key == entry.key;
    });
  }

  template <typename Finder>
  ConstIterator find(unsigned hash, Finder finder) const {
    return items_.find(hash, finder);
  }

private:
  HashTable<HashMapItem<KeyType, ValueType>> items_;
};

}  // namespace nu