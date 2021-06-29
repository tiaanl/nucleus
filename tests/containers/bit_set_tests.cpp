
#include <catch2/catch.hpp>

#include "nucleus/containers/bit_set.h"

namespace nu {

TEST_CASE("BitSet") {
  SECTION("EnoughSpaceForAllBits") {
#if ARCH(CPU_64_BITS)
    CHECK(sizeof(BitSet<8>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<16>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<32>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<49>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<64>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<65>{}) == static_cast<MemSize>(16));
    CHECK(sizeof(BitSet<128>{}) == static_cast<MemSize>(16));
    CHECK(sizeof(BitSet<256>{}) == static_cast<MemSize>(32));
    CHECK(sizeof(BitSet<512>{}) == static_cast<MemSize>(64));
    CHECK(sizeof(BitSet<1024>{}) == static_cast<MemSize>(128));
    CHECK(sizeof(BitSet<2048>{}) == static_cast<MemSize>(256));
    CHECK(sizeof(BitSet<4096>{}) == static_cast<MemSize>(512));
#elif ARCH(CPU_32_BITS)
    CHECK(sizeof(BitSet<8>{}) == static_cast<MemSize>(4));
    CHECK(sizeof(BitSet<16>{}) == static_cast<MemSize>(4));
    CHECK(sizeof(BitSet<32>{}) == static_cast<MemSize>(4));
    CHECK(sizeof(BitSet<49>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<64>{}) == static_cast<MemSize>(8));
    CHECK(sizeof(BitSet<65>{}) == static_cast<MemSize>(12));
    CHECK(sizeof(BitSet<128>{}) == static_cast<MemSize>(16));
    CHECK(sizeof(BitSet<256>{}) == static_cast<MemSize>(32));
    CHECK(sizeof(BitSet<512>{}) == static_cast<MemSize>(64));
    CHECK(sizeof(BitSet<1024>{}) == static_cast<MemSize>(128));
    CHECK(sizeof(BitSet<2048>{}) == static_cast<MemSize>(256));
    CHECK(sizeof(BitSet<4096>{}) == static_cast<MemSize>(512));
#endif
  }

  SECTION("IsAllZeroAfterConstruction") {
    BitSet<32> bits;

    for (BitSet<32>::IndexType i = 0; i < bits.bit_count(); ++i) {
      CHECK_FALSE(bits.test(i));
    }
  }

  SECTION("SetAndTest") {
    BitSet<8> bits;

    bits.set(1);
    CHECK_FALSE(bits.test(0));
    CHECK(bits.test(1));
    CHECK_FALSE(bits.test(2));

    bits.set(1, false);
    CHECK_FALSE(bits.test(0));
    CHECK_FALSE(bits.test(1));
    CHECK_FALSE(bits.test(2));
  }
}

}  // namespace nu
