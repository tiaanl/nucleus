#include <catch2/catch.hpp>

#include "nucleus/byte_order.h"

namespace nu {

namespace {

const U16 BIT_TEST_DATA_16 = 0xAABB;
const U32 BIT_TEST_DATA_32 = 0xAABBCCDD;
const U64 BIT_TEST_DATA_64 = 0xAABBCCDD44332211;

const U16 BIT_SWAPPED_TEST_DATA_16 = 0xBBAA;
const U32 BIT_SWAPPED_TEST_DATA_32 = 0xDDCCBBAA;
const U64 BIT_SWAPPED_TEST_DATA_64 = 0x11223344DDCCBBAA;

}  // namespace

TEST_CASE("ByteOrder") {
  SECTION("byte_swap 16") {
    U16 swapped = byte_swap(BIT_TEST_DATA_16);
    CHECK(swapped == BIT_SWAPPED_TEST_DATA_16);
    U16 reswapped = byte_swap(swapped);
    CHECK(reswapped == BIT_TEST_DATA_16);
  }

  SECTION("byte_swap 32") {
    uint32_t swapped = byte_swap(BIT_TEST_DATA_32);
    CHECK(swapped == BIT_SWAPPED_TEST_DATA_32);
    uint32_t reswapped = byte_swap(swapped);
    CHECK(reswapped == BIT_TEST_DATA_32);
  }

  SECTION("byte_swap 64") {
    uint64_t swapped = byte_swap(BIT_TEST_DATA_64);
    CHECK(swapped == BIT_SWAPPED_TEST_DATA_64);
    uint64_t reswapped = byte_swap(swapped);
    CHECK(reswapped == BIT_TEST_DATA_64);
  }

  SECTION("byte_swap if big endian 16") {
    U16 little_endian = byte_swap_if_big_endian(BIT_TEST_DATA_16);
#if ARCH(CPU_LITTLE_ENDIAN)
    CHECK(little_endian == BIT_TEST_DATA_16);
#else
    CHECK(little_endian == BIT_SWAPPED_TEST_DATA_16);
#endif
  }

  SECTION("byte_swap if big endian 32") {
    U32 little_endian = byte_swap_if_big_endian(BIT_TEST_DATA_32);
#if ARCH(CPU_LITTLE_ENDIAN)
    CHECK(little_endian == BIT_TEST_DATA_32);
#else
    CHECK(little_endian == BIT_SWAPPED_TEST_DATA_32);
#endif
  }

  SECTION("byte_swap if big endian 64") {
    U64 little_endian = byte_swap_if_big_endian(BIT_TEST_DATA_64);
#if ARCH(CPU_LITTLE_ENDIAN)
    CHECK(little_endian == BIT_TEST_DATA_64);
#else
    CHECK(little_endian == BIT_SWAPPED_TEST_DATA_64);
#endif
  }
}

}  // namespace nu
