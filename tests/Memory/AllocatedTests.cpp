
#include "gtest/gtest.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Allocators/TestAllocator.h"
#include "nucleus/Memory/Allocated.h"

TEST(AllocatedTests, Basic) {
  nu::TestAllocator alloc;

  {
    auto i1 = alloc.construct<int>(10);
    auto i2 = alloc.construct<int>(20);

    ASSERT_EQ(2, alloc.getAllocations());
  }

  ASSERT_EQ(2, alloc.getFrees());
}

TEST(AllocatedTests, Get) {
  nu::TestAllocator alloc;

  auto i1 = alloc.construct<int>(10);

  int* i = i1.get();

  EXPECT_EQ(10, *i);
}

TEST(AllocatedTests, Not) {
  nu::TestAllocator alloc;

  auto i1 = alloc.construct<int>(10);

  EXPECT_FALSE(!i1);
}
