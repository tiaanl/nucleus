
#include "gtest/gtest.h"
#include "nucleus/Containers/BitSet.h"

#include <bitset>

TEST(BitSetTests, EnoughSpaceForAllBits) {
#if ARCH(CPU_64_BITS)
  EXPECT_EQ(8, sizeof(nu::BitSet<8>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<16>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<32>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<49>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<64>{}));
  EXPECT_EQ(16, sizeof(nu::BitSet<65>{}));
  EXPECT_EQ(16, sizeof(nu::BitSet<128>{}));
  EXPECT_EQ(32, sizeof(nu::BitSet<256>{}));
  EXPECT_EQ(64, sizeof(nu::BitSet<512>{}));
  EXPECT_EQ(128, sizeof(nu::BitSet<1024>{}));
  EXPECT_EQ(256, sizeof(nu::BitSet<2048>{}));
  EXPECT_EQ(512, sizeof(nu::BitSet<4096>{}));
#elif ARCH(CPU_32_BITS)
  EXPECT_EQ(4, sizeof(nu::BitSet<8>{}));
  EXPECT_EQ(4, sizeof(nu::BitSet<16>{}));
  EXPECT_EQ(4, sizeof(nu::BitSet<32>{}));
  EXPECT_EQ(4, sizeof(nu::BitSet<49>{}));
  EXPECT_EQ(4, sizeof(nu::BitSet<64>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<65>{}));
  EXPECT_EQ(8, sizeof(nu::BitSet<128>{}));
  EXPECT_EQ(16, sizeof(nu::BitSet<256>{}));
  EXPECT_EQ(32, sizeof(nu::BitSet<512>{}));
  EXPECT_EQ(64, sizeof(nu::BitSet<1024>{}));
  EXPECT_EQ(128, sizeof(nu::BitSet<2048>{}));
  EXPECT_EQ(256, sizeof(nu::BitSet<4096>{}));
#endif
}

TEST(BitSetTests, IsAllZeroAfterConstruction) {
  nu::BitSet<32> bits;

  for (nu::BitSet<32>::IndexType i = 0; i < bits.getBitCount(); ++i) {
    ASSERT_FALSE(bits.test(i));
  }
}

TEST(BitSetTests, SetAndTest) {
  std::bitset<10> b;
  b.reset();

  nu::BitSet<8> bits;

  bits.set(1);
  ASSERT_FALSE(bits.test(0));
  ASSERT_TRUE(bits.test(1));
  ASSERT_FALSE(bits.test(2));

  bits.set(1, false);
  ASSERT_FALSE(bits.test(0));
  ASSERT_FALSE(bits.test(1));
  ASSERT_FALSE(bits.test(2));
}
