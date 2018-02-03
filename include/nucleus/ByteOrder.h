
#ifndef NUCLEUS_UTILS_BYTE_ORDER_H_
#define NUCLEUS_UTILS_BYTE_ORDER_H_

#include "nucleus/Config.h"

#if OS(ANDROID)
#include <byteswap.h>
#endif

#if OS(MACOSX)
#include <libkern/OSByteOrder.h>
#endif

namespace nu {

struct ByteOrder {
  // Swaps the upper and lower bytes of an integer.
  static U16 swap(U16 value);
  static U32 swap(U32 value);
  static U64 swap(U64 value);

  // Swaps the byte order of an integer if the CPU is big-endian.
  static U16 swapIfBigEndian(U16 value);
  static U32 swapIfBigEndian(U32 value);
  static U64 swapIfBigEndian(U64 value);

  // Swaps the byte order of an integer if the CPU is little-endian.
  static U16 swapIfLittleEndian(U16 value);
  static U32 swapIfLittleEndian(U32 value);
  static U64 swapIfLittleEndian(U64 value);

  // Turns a number of bytes into a little-endian integer.
  static U16 littleEndianInt16(const void* bytes);
  static U32 littleEndianInt32(const void* bytes);

  // Turns a number of bytes into a big-endian integer.
  static U16 bigEndianInt16(const void* bytes);
  static U32 bigEndianInt32(const void* bytes);

  // Returns true if the current CPU is big-endian.
  static bool isBigEndian();
};

inline U16 ByteOrder::swap(U16 value) {
  return static_cast<U16>((value << 8) | (value >> 8));
}

inline U32 ByteOrder::swap(U32 value) {
#if OS(MACOSX) || OS(IOS)
  return OSSwapInt32(value);
#elif COMPILER(GCC) && ARCH(CPU_X86_FAMILY)
  asm("bswap %%eax" : "=a"(value) : "a"(value));
  return value;
#elif COMPILER(MSVC) && ARCH(CPU_32_BITS)
  // clang-format off
  __asm {
    mov eax, value
    bswap eax
    mov value, eax
  }
  return value;
// clang-format on
#elif OS(ANDROID)
  return bswap_32(value);
#else
  return (value << 24) | (value >> 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8);
#endif
}

inline U64 ByteOrder::swap(U64 value) {
#if OS(MACOSX) || OS(IOS)
  return OSSwapInt64(value);
#else
  return ((static_cast<I64>(swap(static_cast<U32>(value)))) << 32) | swap(static_cast<U32>(value >> 32));
#endif
}

#if ARCH(CPU_LITTLE_ENDIAN)

inline U16 ByteOrder::swapIfBigEndian(const U16 value) {
  return value;
}

inline U32 ByteOrder::swapIfBigEndian(const U32 value) {
  return value;
}

inline U64 ByteOrder::swapIfBigEndian(const U64 value) {
  return value;
}

inline U16 ByteOrder::swapIfLittleEndian(const U16 value) {
  return swap(value);
}

inline U32 ByteOrder::swapIfLittleEndian(const U32 value) {
  return swap(value);
}

inline U64 ByteOrder::swapIfLittleEndian(const U64 value) {
  return swap(value);
}

inline U16 ByteOrder::littleEndianInt16(const void* bytes) {
  return *static_cast<const U16*>(bytes);
}

inline U32 ByteOrder::littleEndianInt32(const void* bytes) {
  return *static_cast<const U32*>(bytes);
}

inline U16 ByteOrder::bigEndianInt16(const void* bytes) {
  return swap(*static_cast<const U16*>(bytes));
}

inline U32 ByteOrder::bigEndianInt32(const void* bytes) {
  return swap(*static_cast<const U32*>(bytes));
}

inline bool ByteOrder::isBigEndian() {
  return false;
}

#elif ARCH(CPU_BIG_ENDIAN)

inline U16 ByteOrder::swapIfBigEndian(const U16 value) {
  return swap(value);
}

inline U32 ByteOrder::swapIfBigEndian(const U32 value) {
  return swap(value);
}

inline u64 ByteOrder::swapIfBigEndian(const u64 value) {
  return swap(value);
}

inline U16 ByteOrder::swapIfLittleEndian(const U16 value) {
  return value;
}

inline U32 ByteOrder::swapIfLittleEndian(const U32 value) {
  return value;
}

inline u64 ByteOrder::swapIfLittleEndian(const u64 value) {
  return value;
}

inline U16 ByteOrder::littleEndianInt16(const void* bytes) {
  return swap(*static_cast<const U16*>(bytes));
}

inline U32 ByteOrder::littleEndianInt32(const void* bytes) {
  return swap(*static_cast<const U32*>(bytes));
}

inline U16 ByteOrder::bigEndianInt16(const void* bytes) {
  return *static_cast<const U16*>(bytes);
}

inline U32 ByteOrder::bigEndianInt32(const void* bytes) {
  return *static_cast<const U32*>(bytes);
}

inline bool ByteOrder::isBigEndian() {
  return true;
}

#else
#error "Can't figure out what endian-ess you platform is."
#endif

}  // namespace nu

#endif  // NUCLEUS_UTILS_BYTE_ORDER_H_
