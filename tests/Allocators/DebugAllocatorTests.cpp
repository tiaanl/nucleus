
#include "gtest/gtest.h"
#include "nucleus/Allocators/DebugAllocator.h"

void expectDebugValues(const nu::DebugAllocator& debug, USize bytesAllocated, USize bytesFreed, USize bytesOutstanding,
                       USize maxAllocated, USize blocksOutstanding) {
  EXPECT_EQ(bytesAllocated, debug.getBytesAllocated());
  EXPECT_EQ(bytesFreed, debug.getBytesFreed());
  EXPECT_EQ(bytesOutstanding, debug.getBytesOutstanding());
  EXPECT_EQ(maxAllocated, debug.getMaxAllocated());
  EXPECT_EQ(blocksOutstanding, debug.getBlocksOutstanding());
}

TEST(DebugAllocatorTests, Basic) {
  nu::GlobalAllocator global;
  nu::DebugAllocator debug{&global};

  void* p1 = debug.allocate(16, 16);
  expectDebugValues(debug, 16, 0, 16, 16, 1);

  debug.free(p1, 16, 16);
  expectDebugValues(debug, 16, 16, 0, 16, 0);
}
