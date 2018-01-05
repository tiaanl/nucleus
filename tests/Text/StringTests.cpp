
#include "gtest/gtest.h"
#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Text/String.h"

TEST(StringTests, Allocation) {
  nu::DebugAllocator debug;

  { nu::String{"test", 4, &debug}; }

  EXPECT_EQ(0, debug.getBytesOutstanding());
}
