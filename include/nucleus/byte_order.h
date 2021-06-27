#pragma once

#include "nucleus/config.h"
#include "nucleus/types.h"

#if OS(ANDROID)
#include <byteswap.h>
#endif

#if OS(MACOSX)
#include <libkern/OSByteOrder.h>
#endif

namespace nu {

inline constexpr U16 byte_swap(U16 value) {
  return static_cast<U16>((value << 8) | (value >> 8));
}

inline constexpr U32 byte_swap(U32 value) {
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

inline constexpr U64 byte_swap(U64 value) {
#if OS(MACOSX) || OS(IOS)
  return OSSwapInt64(value);
#else
  return ((static_cast<I64>(byte_swap(static_cast<U32>(value)))) << 32) |
         byte_swap(static_cast<U32>(value >> 32));
#endif
}

#if ARCH(CPU_LITTLE_ENDIAN)

inline constexpr U16 byte_swap_if_big_endian(U16 value) {
  return value;
}

inline constexpr U32 byte_swap_if_big_endian(U32 value) {
  return value;
}

inline constexpr U64 byte_swap_if_big_endian(U64 value) {
  return value;
}

inline constexpr U16 byte_swap_if_little_endian(U16 value) {
  return byte_swap(value);
}

inline constexpr U32 byte_swap_if_little_endian(U32 value) {
  return byte_swap(value);
}

inline constexpr U64 byte_swap_if_little_endian(U64 value) {
  return byte_swap(value);
}

inline U16 little_endian_16(const void* bytes) {
  return *static_cast<const U16*>(bytes);
}

inline U32 little_endian_32(const void* bytes) {
  return *static_cast<const U32*>(bytes);
}

inline U16 big_endian_16(const void* bytes) {
  return byte_swap(*static_cast<const U16*>(bytes));
}

inline U32 big_endian_32(const void* bytes) {
  return byte_swap(*static_cast<const U32*>(bytes));
}

inline constexpr bool is_big_endian() {
  return false;
}

inline constexpr bool is_little_endian() {
  return true;
}

#elif ARCH(CPU_BIG_ENDIAN)

inline constexpr U16 byte_swap_if_big_endian(U16 value) {
  return swap(value);
}

inline constexpr U32 byte_swap_if_big_endian(U32 value) {
  return swap(value);
}

inline constexpr U64 byte_swap_if_big_endian(U64 value) {
  return swap(value);
}

inline constexpr U16 byte_swap_if_little_endian(U16 value) {
  return value;
}

inline constexpr U32 byte_swap_if_little_endian(U32 value) {
  return value;
}

inline constexpr U64 byte_swap_if_little_endian(U64 value) {
  return value;
}

inline U16 little_endian_16(const void* bytes) {
  return swap(*static_cast<const U16*>(bytes));
}

inline U32 little_endian_32(const void* bytes) {
  return swap(*static_cast<const U32*>(bytes));
}

inline U16 big_endian_16(const void* bytes) {
  return *static_cast<const U16*>(bytes);
}

inline U32 big_endian_32(const void* bytes) {
  return *static_cast<const U32*>(bytes);
}

inline constexpr bool is_big_endian() {
  return true;
}

inline constexpr bool is_little_endian() {
  return false;
}

#else
#error "Can't figure out what endian-ess you platform is."
#endif

}  // namespace nu
