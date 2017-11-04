
#include <gtest/gtest.h>

#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Containers/Vector.h"

TEST(VectorTests, DefaultConstruct) {
  nu::Vector<int> v;
}

struct SomeData {
  int a;
  int b;

  SomeData(int a, int b) : a(a), b(b) {}
};

TEST(VectorTests, PushBack) {
  nu::Vector<SomeData> v;
  v.pushBack(SomeData{10, 20});
  EXPECT_EQ(1, v.getSize());
}
