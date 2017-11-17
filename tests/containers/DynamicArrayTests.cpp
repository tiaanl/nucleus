
#include "nucleus/Containers/DynamicArray.h"

#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/types.h"

#include "gtest/gtest.h"

TEST(DynamicArrayTests, Basic) {
  nu::DynamicArray<U8> buffer;
  buffer.pushBack(10);

  ASSERT_EQ(1, buffer.getSize());
  ASSERT_EQ(10, buffer.get(0));
}

TEST(DynamicArrayTests, CopyConstruct) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U32> buffer2{buffer};

  ASSERT_EQ(buffer.getSize(), buffer2.getSize());
  ASSERT_EQ(buffer.get(0), buffer2.get(0));
  ASSERT_EQ(buffer.get(1), buffer2.get(1));
}

TEST(DynamicArrayTests, CopyAssignment) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U32> buffer2;
  ASSERT_EQ(0, buffer2.getSize());

  buffer2 = buffer;

  ASSERT_EQ(buffer.getSize(), buffer2.getSize());
  ASSERT_EQ(buffer.get(0), buffer2.get(0));
  ASSERT_EQ(buffer.get(1), buffer2.get(1));
}

TEST(DynamicArrayTests, MoveConstruct) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U32> buffer2{std::move(buffer)};

  ASSERT_EQ(0, buffer.getSize());
  ASSERT_EQ(2, buffer2.getSize());
  ASSERT_EQ(10, buffer2.get(0));
  ASSERT_EQ(20, buffer2.get(1));
}

TEST(DynamicArrayTests, MoveAssignment) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U32> buffer2;
  ASSERT_EQ(0, buffer2.getSize());

  buffer2 = std::move(buffer);

  ASSERT_EQ(0, buffer.getSize());
  ASSERT_EQ(2, buffer2.getSize());
  ASSERT_EQ(10, buffer2.get(0));
  ASSERT_EQ(20, buffer2.get(1));
}

class LifetimeType {
public:
  static I32 creates;
  static I32 destroys;
  static I32 copies;

  static void reset() {
    LifetimeType::creates = 0;
    LifetimeType::destroys = 0;
    LifetimeType::copies = 0;
  }

  LifetimeType(I32 a, I32 b) : m_a(a), m_b(b) {
    creates++;
  }

  LifetimeType(const LifetimeType& other) : m_a(other.m_a), m_b(other.m_b) {
    creates++;
  }

  ~LifetimeType() {
    destroys++;
  }

  LifetimeType& operator=(const LifetimeType& other) {
    m_a = other.m_a;
    m_b = other.m_b;

    copies++;

    return *this;
  }

  I32 getA() const {
    return m_a;
  }

  I32 getB() const {
    return m_b;
  }

private:
  I32 m_a;
  I32 m_b;
};

I32 LifetimeType::creates = 0;
I32 LifetimeType::destroys = 0;
I32 LifetimeType::copies = 0;

TEST(DynamicArrayTests, EmplaceBack) {
  nu::DynamicArray<LifetimeType> buffer;
  buffer.pushBack(LifetimeType{1, 2});
  buffer.emplaceBack(3, 4);

  ASSERT_EQ(2, buffer.getSize());
  ASSERT_EQ(1, buffer.get(0).getA());
  ASSERT_EQ(2, buffer.get(0).getB());
  ASSERT_EQ(3, buffer.get(1).getA());
  ASSERT_EQ(4, buffer.get(1).getB());
}

TEST(DynamicArrayTests, CreateAndDestroyElements) {
  LifetimeType::reset();

  {
    nu::DynamicArray<LifetimeType> buffer;
    buffer.pushBack(LifetimeType{1, 2});
    buffer.emplaceBack(3, 4);
  }

  // The temporary is created before pushing it back.
  // Inside emplace back it is created inplace.
  ASSERT_EQ(2, LifetimeType::creates);

  // The temporary which was pushed back.
  // The 2 elements in the array when it is destroyed.
  ASSERT_EQ(3, LifetimeType::destroys);

  // Once when the temporary in copied into the array when pushed back.
  ASSERT_EQ(1, LifetimeType::copies);
}

void dynamicArrayWithoutConst(nu::DynamicArray<U32>& buffer) {
  USize i = 0;
  for (auto& e : buffer) {
    ASSERT_EQ((i + 1) * 10, buffer.get(i));
    i++;
  }
}

void dynamicArrayWithConst(const nu::DynamicArray<U32>& buffer) {
  USize i = 0;
  for (auto& e : buffer) {
    ASSERT_EQ((i + 1) * 10, buffer.get(i));
    i++;
  }
}

TEST(DynamicArrayTests, RangedBasedForLoops) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);

  dynamicArrayWithoutConst(buffer);
  dynamicArrayWithConst(buffer);
}

TEST(DynamicArrayTests, Remove) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);
  buffer.pushBack(40);

  buffer.remove(buffer.begin() + 1);

  ASSERT_EQ(3, buffer.getSize());
  ASSERT_EQ(10, buffer.get(0));
  ASSERT_EQ(30, buffer.get(1));
  ASSERT_EQ(buffer.end(), &buffer.get(3));
}

TEST(DynamicArrayTests, RemoveCallsDestructor) {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  buffer.remove(buffer.begin() + 1);

  ASSERT_EQ(4, LifetimeType::creates);
  ASSERT_EQ(1, LifetimeType::destroys);
  ASSERT_EQ(0, LifetimeType::copies);
}

TEST(DynamicArrayTests, RemoveRange) {
  nu::DynamicArray<U32> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);
  buffer.pushBack(40);

  buffer.remove(buffer.begin() + 1, buffer.begin() + 3);

  ASSERT_EQ(2, buffer.getSize());
  ASSERT_EQ(10, buffer.get(0));
  ASSERT_EQ(40, buffer.get(1));
  ASSERT_EQ(buffer.end(), &buffer.get(2));
}

TEST(DynamicArrayTests, RemoveRangeCallsDestructors) {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  buffer.remove(buffer.begin() + 1, buffer.begin() + 3);

  ASSERT_EQ(4, LifetimeType::creates);
  ASSERT_EQ(2, LifetimeType::destroys);
  ASSERT_EQ(0, LifetimeType::copies);
}

TEST(DynamicArrayTests, Clear) {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  ASSERT_EQ(4, buffer.getSize());

  buffer.clear();

  ASSERT_EQ(0, buffer.getSize());

  ASSERT_EQ(4, LifetimeType::creates);
  ASSERT_EQ(4, LifetimeType::destroys);
  ASSERT_EQ(0, LifetimeType::copies);
}
