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
  static u16 swap(u16 value);
  static u32 swap(u32 value);
  static u64 swap(u64 value);

  // Swaps the byte order of an integer if the CPU is big-endian.
  static u16 swapIfBigEndian(u16 value);
  static u32 swapIfBigEndian(u32 value);
  static u64 swapIfBigEndian(u64 value);

  // Swaps the byte order of an integer if the CPU is little-endian.
  static u16 swapIfLittleEndian(u16 value);
  static u32 swapIfLittleEndian(u32 value);
  static u64 swapIfLittleEndian(u64 value);

  // Turns a number of bytes into a little-endian integer.
  static u16 littleEndianInt16(const void* bytes);
  static u32 littleEndianInt32(const void* bytes);

  // Turns a number of bytes into a big-endian integer.
  static u16 bigEndianInt16(const void* bytes);
  static u32 bigEndianInt32(const void* bytes);

  // Returns true if the current CPU is big-endian.
  static bool isBigEndian();
};

inline u16 ByteOrder::swap(u16 value) {
  return static_cast<u16>((value << 8) | (value >> 8));
}

inline u32 ByteOrder::swap(u32 value) {
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

inline u64 ByteOrder::swap(u64 value) {
#if OS(MACOSX) || OS(IOS)
  return OSSwapInt64(value);
#else
  return ((static_cast<i64>(swap(static_cast<u32>(value)))) << 32) |
         swap(static_cast<u32>(value >> 32));
#endif
}

#if ARCH(CPU_LITTLE_ENDIAN)

inline u16 ByteOrder::swapIfBigEndian(const u16 value) {
  return value;
}

inline u32 ByteOrder::swapIfBigEndian(const u32 value) {
  return value;
}

inline u64 ByteOrder::swapIfBigEndian(const u64 value) {
  return value;
}

inline u16 ByteOrder::swapIfLittleEndian(const u16 value) {
  return swap(value);
}

inline u32 ByteOrder::swapIfLittleEndian(const u32 value) {
  return swap(value);
}

inline u64 ByteOrder::swapIfLittleEndian(const u64 value) {
  return swap(value);
}

inline u16 ByteOrder::littleEndianInt16(const void* bytes) {
  return *static_cast<const u16*>(bytes);
}

inline u32 ByteOrder::littleEndianInt32(const void* bytes) {
  return *static_cast<const u32*>(bytes);
}

inline u16 ByteOrder::bigEndianInt16(const void* bytes) {
  return swap(*static_cast<const u16*>(bytes));
}

inline u32 ByteOrder::bigEndianInt32(const void* bytes) {
  return swap(*static_cast<const u32*>(bytes));
}

inline bool ByteOrder::isBigEndian() {
  return false;
}

#elif ARCH(CPU_BIG_ENDIAN)

inline u16 ByteOrder::swapIfBigEndian(const u16 value) {
  return swap(value);
}

inline u32 ByteOrder::swapIfBigEndian(const u32 value) {
  return swap(value);
}

inline u64 ByteOrder::swapIfBigEndian(const u64 value) {
  return swap(value);
}

inline u16 ByteOrder::swapIfLittleEndian(const u16 value) {
  return value;
}

inline u32 ByteOrder::swapIfLittleEndian(const u32 value) {
  return value;
}

inline u64 ByteOrder::swapIfLittleEndian(const u64 value) {
  return value;
}

inline u16 ByteOrder::littleEndianInt16(const void* bytes) {
  return swap(*static_cast<const u16*>(bytes));
}

inline u32 ByteOrder::littleEndianInt32(const void* bytes) {
  return swap(*static_cast<const u32*>(bytes));
}

inline u16 ByteOrder::bigEndianInt16(const void* bytes) {
  return *static_cast<const u16*>(bytes);
}

inline u32 ByteOrder::bigEndianInt32(const void* bytes) {
  return *static_cast<const u32*>(bytes);
}

inline bool ByteOrder::isBigEndian() {
  return true;
}

#else
#error "Can't figure out what endian-ess you platform is."
#endif

}  // namespace nu

#endif  // NUCLEUS_UTILS_BYTE_ORDER_H_
