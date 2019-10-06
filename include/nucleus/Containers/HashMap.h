#ifndef NUCLEUS_CONTAINERS_HASH_MAP_H_
#define NUCLEUS_CONTAINERS_HASH_MAP_H_

#include "nucleus/Containers/DynamicArray.h"

namespace nu {

template <typename K, typename V, MemSize HashSize = 64>
class HashMap {
public:
  using KeyType = K;
  using ValueType = V;

  MemSize size() const {
    return m_keys.size();
  }

  class InsertResult {
  };

  InsertResult insert(const KeyType& key, const ValueType& value) {
    auto hash = hashOf(key);
  }

  class FindResult {
  };

  FindResult find(const KeyType& key) const {
    auto hash = hashOf(key);
  }

private:
  struct Key {
    KeyType key;
    MemSize valueIndex;
  };

  nu::DynamicArray<Key> m_keys;
  nu::DynamicArray<ValueType> m_values;
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_HASH_MAP_H_
