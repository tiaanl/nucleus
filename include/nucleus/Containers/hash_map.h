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
struct HashMapItemTraits {
  using ItemType = HashMapItem<KeyType, ValueType>;

  static HashedValue hashed(const ItemType& item) {
    return Hash<KeyType>::hashed(item.key);
  }

  static bool equals(const ItemType& left, const ItemType& right) {
    return left.key == right.key;
  }
};

template <typename KeyType, typename ValueType>
class HashMap
  : public HashTableBase<HashMapItem<KeyType, ValueType>, HashMapItemTraits<KeyType, ValueType>> {
public:
  using ItemType = HashMapItem<KeyType, ValueType>;

  HashMap() = default;

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

  InsertResult insert(KeyType key, ValueType value) {
    auto hash = Hash<KeyType>::hashed(key);
    auto* bucket = this->find_bucket_for_writing(hash, [&](ItemType& i) {
      return i.key == key;
    });
    DCHECK(bucket) << "Could not find a bucket for writing.";

    bool is_new = !bucket->is_used();

    auto item = ItemType{std::move(key), std::move(value)};
    bucket->set(std::move(item));

    if (is_new) {
      ++this->size_;
    }

    return {is_new, &bucket->reference().key, &bucket->reference().value};
  }

  bool contains_key(const KeyType& key) const {
    return find(key) != this->end();
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
    auto* bucket = this->find_bucket_for_reading(Hash<KeyType>::hashed(key), [&](ItemType& item) {
      return item.key == key;
    });
    if (bucket) {
      return {true, &bucket->reference().key, &bucket->reference().value};
    }

    return {false, nullptr, nullptr};
  }
};

}  // namespace nu
