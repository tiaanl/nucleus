
#include "nucleus/Containers/BitSet.h"
#include "nucleus/Testing.h"

TEST_CASE("EnoughSpaceForAllBits") {
#if ARCH(CPU_64_BITS)
  CHECK(sizeof(nu::BitSet<8>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<16>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<32>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<49>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<64>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<65>{}) == static_cast<USize>(16));
  CHECK(sizeof(nu::BitSet<128>{}) == static_cast<USize>(16));
  CHECK(sizeof(nu::BitSet<256>{}) == static_cast<USize>(32));
  CHECK(sizeof(nu::BitSet<512>{}) == static_cast<USize>(64));
  CHECK(sizeof(nu::BitSet<1024>{}) == static_cast<USize>(128));
  CHECK(sizeof(nu::BitSet<2048>{}) == static_cast<USize>(256));
  CHECK(sizeof(nu::BitSet<4096>{}) == static_cast<USize>(512));
#elif ARCH(CPU_32_BITS)
  CHECK(sizeof(nu::BitSet<8>{}) == static_cast<USize>(4));
  CHECK(sizeof(nu::BitSet<16>{}) == static_cast<USize>(4));
  CHECK(sizeof(nu::BitSet<32>{}) == static_cast<USize>(4));
  CHECK(sizeof(nu::BitSet<49>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<64>{}) == static_cast<USize>(8));
  CHECK(sizeof(nu::BitSet<65>{}) == static_cast<USize>(12));
  CHECK(sizeof(nu::BitSet<128>{}) == static_cast<USize>(16));
  CHECK(sizeof(nu::BitSet<256>{}) == static_cast<USize>(32));
  CHECK(sizeof(nu::BitSet<512>{}) == static_cast<USize>(64));
  CHECK(sizeof(nu::BitSet<1024>{}) == static_cast<USize>(128));
  CHECK(sizeof(nu::BitSet<2048>{}) == static_cast<USize>(256));
  CHECK(sizeof(nu::BitSet<4096>{}) == static_cast<USize>(512));
#endif
}

TEST_CASE("IsAllZeroAfterConstruction") {
  nu::BitSet<32> bits;

  for (nu::BitSet<32>::IndexType i = 0; i < bits.getBitCount(); ++i) {
    REQUIRE_FALSE(bits.test(i));
  }
}

TEST_CASE("SetAndTest") {
  nu::BitSet<8> bits;

  bits.set(1);
  CHECK(!bits.test(0));
  CHECK(bits.test(1));
  CHECK(!bits.test(2));

  bits.set(1, false);
  CHECK(!bits.test(0));
  CHECK(!bits.test(1));
  CHECK(!bits.test(2));
}
