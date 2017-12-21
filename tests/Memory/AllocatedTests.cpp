
#include "gtest/gtest.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Allocators/TestAllocator.h"
#include "nucleus/Memory/Allocated.h"

TEST(AllocatedTests, Basic) {
  nu::TestAllocator alloc;

  {
    auto i1 = nu::allocate<int>(&alloc, 10);
    auto i2 = nu::allocate<int>(&alloc, 20);

    ASSERT_EQ(2, alloc.getAllocations());
  }

  ASSERT_EQ(2, alloc.getFrees());
}

TEST(AllocatedTests, Get) {
  nu::TestAllocator alloc;

  auto i1 = nu::allocate<int>(&alloc, 10);

  int* i = i1.get();

  EXPECT_EQ(10, *i);
}

TEST(AllocatedTests, Not) {
  nu::TestAllocator alloc;

  auto i1 = nu::allocate<int>(&alloc, 10);

  EXPECT_FALSE(!i1);
}

TEST(AllocatedTests, Move) {
  nu::TestAllocator alloc;

  {
    auto i1 = nu::allocate<int>(&alloc, 10);

    nu::Allocated<int> i2{&alloc};
    i2 = nu::move(i1);

    ASSERT_EQ(nullptr, i1.get());
    ASSERT_FALSE(!i2);
  }

  {
    auto i1 = nu::allocate<int>(&alloc, 10);
    auto i2{nu::move(i1)};

    ASSERT_EQ(nullptr, i1.get());
    ASSERT_FALSE(!i2);
  }
}
