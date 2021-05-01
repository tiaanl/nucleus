#pragma once

#include "nucleus/Containers/hash_table.h"

namespace nu {

template <typename KeyType, typename ValueType>
struct HashMapItem {
  KeyType key;
  ValueType value;

  friend bool operator==(const HashMapItem& left, const HashMapItem& right) {
    return left.key == right.key;
  }
};

template <typename KeyType, typename ValueType>
struct Hash<HashMapItem<KeyType, ValueType>> {
  static HashedValue hashed(const HashMapItem<KeyType, ValueType>& item) {
    return Hash<KeyType>::hashed(item.key);
  }
};

template <typename KeyType, typename ValueType>
class HashMap {
public:
  using ItemType = HashMapItem<KeyType, ValueType>;
  using Iterator = typename HashTable<ItemType>::Iterator;
  using ConstIterator = typename HashTable<ItemType>::Iterator;

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

  class InsertResult {
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

    InsertResult(bool is_new, KeyType* key, ValueType* value)
      : is_new_{is_new}, key_{key}, value_{value} {}

    bool is_new_;
    KeyType* key_;
    ValueType* value_;
  };

  InsertResult insert(const KeyType& key, ValueType value) {
    auto result = items_.insert({std::move(key), std::move(value)});

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

  class FindResult {
  public:
    bool was_found() const {
      return was_found_;
    }

    const KeyType& key() const {
      DCHECK(key_);
      return *key_;
    }

    ValueType& value() const {
      DCHECK(value_);
      return *value_;
    }

  private:
    friend class HashMap<KeyType, ValueType>;

    FindResult(bool was_found, KeyType* key, ValueType* value)
      : was_found_{was_found}, key_{key}, value_{value} {}

    bool was_found_;
    KeyType* key_;
    ValueType* value_;
  };

  FindResult find(const KeyType& key) {
    auto result = items_.find(Hash<KeyType>::hashed(key), [&](ItemType& item) {
      return key == item.key;
    });
    if (result.was_found()) {
      return {true, &result.item().key, &result.item().value};
    }

    return {false, nullptr, nullptr};
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
  HashTable<ItemType> items_;
};

}  // namespace nu
