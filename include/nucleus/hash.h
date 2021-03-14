#pragma once

#include "nucleus/Types.h"

namespace nu {

using HashedValue = U32;

constexpr HashedValue hash_dword(U32 key) {
  key += ~(key << 15);
  key ^= (key >> 10);
  key += (key << 3);
  key ^= (key >> 6);
  key += ~(key << 11);
  key ^= (key >> 16);

  return key;
}

constexpr HashedValue pair_dword_hash(U32 key1, U32 key2) {
  return hash_dword((hash_dword(key1) * 209) ^ (hash_dword(key2 * 413)));
}

constexpr HashedValue hash_qword(U64 key) {
  U32 first = key & 0xFFFFFFFF;
  U32 last = key >> 32;

  return pair_dword_hash(first, last);
}

// Hashes a previously hashed value.  This is different from an int hash to avoid collisions.
constexpr HashedValue double_hash(HashedValue key) {
  key = ~key + (key >> 23);
  key ^= (key << 12);
  key ^= (key >> 7);
  key ^= (key << 2);
  key ^= (key >> 20);

  return key;
}

template <typename T>
struct Hash;

template <>
struct Hash<I32> {
  static HashedValue hashed(I32 value) {
    return hash_dword(value);
  }
};

template <>
struct Hash<U32> {
  static HashedValue hashed(U32 value) {
    return hash_dword(value);
  }
};

template <>
struct Hash<I64> {
  static HashedValue hashed(I64 value) {
    return hash_qword(value);
  }
};

template <>
struct Hash<U64> {
  static HashedValue hashed(I64 value) {
    return hash_qword(value);
  }
};

}  // namespace nu
