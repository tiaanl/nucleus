
#include "nucleus/Allocators/Allocated.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Allocators/TestAllocator.h"
#include "nucleus/Testing.h"


#if 0
TEST_CASE("allocate an object") {
  nu::TestAllocator alloc;

  {
    auto i1 = nu::allocate<int>(&alloc, 10);
    auto i2 = nu::allocate<int>(&alloc, 20);

    REQUIRE(alloc.getAllocations() == 2);
  }

  REQUIRE(alloc.getFrees() == 2);
}

TEST_CASE("get the allocated object") {
  nu::TestAllocator alloc;

  auto i1 = nu::allocate<int>(&alloc, 10);

  int* i = i1.get();

  CHECK(*i == 10);
}

TEST_CASE("check if Allocated holds an object using negation") {
  nu::TestAllocator alloc;

  auto i1 = nu::allocate<int>(&alloc, 10);

  CHECK_FALSE(!i1);
}

TEST_CASE("move the object into another Allocated") {
  nu::TestAllocator alloc;

  {
    auto i1 = nu::allocate<int>(&alloc, 10);

    nu::Allocated<int> i2{&alloc};
    i2 = std::move(i1);

    REQUIRE(i1.get() == nullptr);
    REQUIRE_FALSE(!i2);
  }

  {
    auto i1 = nu::allocate<int>(&alloc, 10);
    nu::Allocated<int> i2{std::move(i1)};

    REQUIRE(i1.get() == nullptr);
    REQUIRE_FALSE(!i2);
  }
}

TEST_CASE("construct an Allocated with an existing pointer") {
  nu::TestAllocator alloc;

  {
    nu::Allocated<I32> a(&alloc);
    CHECK(a.isEmpty());
    CHECK(alloc.getAllocations() == 0);

    a.allocate(10);
    CHECK(*a == 10);
  }

  CHECK(alloc.getAllocations() == 1);
  CHECK(alloc.getFrees() == 1);
}
#endif  // 0
