// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_UTILS_BYTE_ORDER_H_
#define NUCLEUS_UTILS_BYTE_ORDER_H_

#include <cstdint>

#include "nucleus/config.h"

#if OS(ANDROID)
#include <byteswap.h>
#endif

#if OS(MACOSX)
#include <libkern/OSByteOrder.h>
#endif

namespace nu {

struct ByteOrder {
  // Swaps the upper and lower bytes of an integer.
  static uint16_t swap(uint16_t value);
  static uint32_t swap(uint32_t value);
  static uint64_t swap(uint64_t value);

  // Swaps the byte order of an integer if the CPU is big-endian.
  static uint16_t swapIfBigEndian(uint16_t value);
  static uint32_t swapIfBigEndian(uint32_t value);
  static uint64_t swapIfBigEndian(uint64_t value);

  // Swaps the byte order of an integer if the CPU is little-endian.
  static uint16_t swapIfLittleEndian(uint16_t value);
  static uint32_t swapIfLittleEndian(uint32_t value);
  static uint64_t swapIfLittleEndian(uint64_t value);

  // Turns a number of bytes into a little-endian integer.
  static uint16_t littleEndianInt16(const void* bytes);
  static uint32_t littleEndianInt32(const void* bytes);

  // Turns a number of bytes into a big-endian integer.
  static uint16_t bigEndianInt16(const void* bytes);
  static uint32_t bigEndianInt32(const void* bytes);

  // Returns true if the current CPU is big-endian.
  static bool isBigEndian();
};

inline uint16_t ByteOrder::swap(uint16_t value) {
  return static_cast<uint16_t>((value << 8) | (value >> 8));
}

inline uint32_t ByteOrder::swap(uint32_t value) {
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
  return (value << 24) | (value >> 24) | ((value & 0xff00) << 8) |
         ((value & 0xff0000) >> 8);
#endif
}

inline uint64_t ByteOrder::swap(uint64_t value) {
#if OS(MACOSX) || OS(IOS)
  return OSSwapInt64(value);
#else
  return ((static_cast<int64_t>(swap(static_cast<uint32_t>(value)))) << 32) |
         swap(static_cast<uint32_t>(value >> 32));
#endif
}

#if ARCH(CPU_LITTLE_ENDIAN)

inline uint16_t ByteOrder::swapIfBigEndian(const uint16_t value) {
  return value;
}

inline uint32_t ByteOrder::swapIfBigEndian(const uint32_t value) {
  return value;
}

inline uint64_t ByteOrder::swapIfBigEndian(const uint64_t value) {
  return value;
}

inline uint16_t ByteOrder::swapIfLittleEndian(const uint16_t value) {
  return swap(value);
}

inline uint32_t ByteOrder::swapIfLittleEndian(const uint32_t value) {
  return swap(value);
}

inline uint64_t ByteOrder::swapIfLittleEndian(const uint64_t value) {
  return swap(value);
}

inline uint16_t ByteOrder::littleEndianInt16(const void* bytes) {
  return *static_cast<const uint16_t*>(bytes);
}

inline uint32_t ByteOrder::littleEndianInt32(const void* bytes) {
  return *static_cast<const uint32_t*>(bytes);
}

inline uint16_t ByteOrder::bigEndianInt16(const void* bytes) {
  return swap(*static_cast<const uint16_t*>(bytes));
}

inline uint32_t ByteOrder::bigEndianInt32(const void* bytes) {
  return swap(*static_cast<const uint32_t*>(bytes));
}

inline bool ByteOrder::isBigEndian() {
  return false;
}

#elif ARCH(CPU_BIG_ENDIAN)

inline uint16_t ByteOrder::swapIfBigEndian(const uint16_t value) {
  return swap(value);
}

inline uint32_t ByteOrder::swapIfBigEndian(const uint32_t value) {
  return swap(value);
}

inline uint64_t ByteOrder::swapIfBigEndian(const uint64_t value) {
  return swap(value);
}

inline uint16_t ByteOrder::swapIfLittleEndian(const uint16_t value) {
  return value;
}

inline uint32_t ByteOrder::swapIfLittleEndian(const uint32_t value) {
  return value;
}

inline uint64_t ByteOrder::swapIfLittleEndian(const uint64_t value) {
  return value;
}

inline uint16_t ByteOrder::littleEndianInt16(const void* bytes) {
  return swap(*static_cast<const uint16_t*>(bytes));
}

inline uint32_t ByteOrder::littleEndianInt32(const void* bytes) {
  return swap(*static_cast<const uint32_t*>(bytes));
}

inline uint16_t ByteOrder::bigEndianInt16(const void* bytes) {
  return *static_cast<const uint16_t*>(bytes);
}

inline uint32_t ByteOrder::bigEndianInt32(const void* bytes) {
  return *static_cast<const uint32_t*>(bytes);
}

inline bool ByteOrder::isBigEndian() {
  return true;
}

#else
#error "Can't figure out what endian-ess you platform is."
#endif

}  // namespace nu

#endif  // NUCLEUS_UTILS_BYTE_ORDER_H_
