
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Logging.h"
#include "nucleus/Testing.h"
#include "nucleus/Types.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

TEST_CASE("basic") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);

  REQUIRE(buffer.size() == static_cast<MemSize>(1));
  REQUIRE(buffer[0] == 10);
}

TEST_CASE("copy construct") {
  nu::DynamicArray<U64> buffer1;
  buffer1.pushBack(10);
  buffer1.pushBack(20);

  nu::DynamicArray<U64> buffer2{buffer1};

  REQUIRE(buffer1.size() == buffer2.size());
  REQUIRE(buffer1[0] == buffer2[0]);
  REQUIRE(buffer1[1] == buffer2[1]);
}

TEST_CASE("copy assignment") {
  nu::DynamicArray<U64> buffer1;
  buffer1.pushBack(10);
  buffer1.pushBack(20);

  nu::DynamicArray<U64> buffer2;
  REQUIRE(buffer2.size() == 0);

  buffer2 = buffer1;

  REQUIRE(buffer1.size() == buffer2.size());
  REQUIRE(buffer1[0] == buffer2[0]);
  REQUIRE(buffer1[1] == buffer2[1]);
}

TEST_CASE("move construct") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U64> buffer2{std::move(buffer)};

  REQUIRE(buffer.size() == 0);
  REQUIRE(buffer2.size() == 2);
  REQUIRE(buffer2[0] == 10);
  REQUIRE(buffer2[1] == 20);
}

TEST_CASE("move assignment") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);

  nu::DynamicArray<U64> buffer2;
  REQUIRE(buffer2.size() == 0);

  buffer2 = std::move(buffer);

  REQUIRE(buffer.size() == 0);
  REQUIRE(buffer2.size() == 2);
  REQUIRE(buffer2[0] == 10);
  REQUIRE(buffer2[1] == 20);
}

class LifetimeType {
public:
  static I32 creates;
  static I32 destroys;
  static I32 copies;
  static I32 moves;

  static void reset() {
    LifetimeType::creates = 0;
    LifetimeType::destroys = 0;
    LifetimeType::copies = 0;
    LifetimeType::moves = 0;
  }

  LifetimeType(I32 a, I32 b) : m_a(a), m_b(b) {
    creates++;
  }

  LifetimeType(const LifetimeType& other) : m_a(other.m_a), m_b(other.m_b) {
    copies++;
  }

  LifetimeType(LifetimeType&& other) : m_a(other.m_a), m_b(other.m_b) {
    other.m_a = 0;
    other.m_b = 0;

    moves++;
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

  LifetimeType& operator=(LifetimeType&& other) {
    m_a = other.m_a;
    m_b = other.m_b;

    other.m_a = 0;
    other.m_b = 0;

    moves++;

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
I32 LifetimeType::moves = 0;

TEST_CASE("emplace back") {
  nu::DynamicArray<LifetimeType> buffer;
  buffer.pushBack(LifetimeType{1, 2});
  buffer.emplaceBack(3, 4);

  REQUIRE(buffer.size() == 2);
  REQUIRE(buffer[0].getA() == 1);
  REQUIRE(buffer[0].getB() == 2);
  REQUIRE(buffer[1].getA() == 3);
  REQUIRE(buffer[1].getB() == 4);
}

TEST_CASE("create and destroy elements") {
  LifetimeType::reset();

  {
    nu::DynamicArray<LifetimeType> buffer;
    buffer.pushBack(LifetimeType{1, 2});
  }

  REQUIRE(LifetimeType::creates == 1);
  REQUIRE(LifetimeType::destroys == 2);
  REQUIRE(LifetimeType::copies == 0);
  REQUIRE(LifetimeType::moves == 1);

  LifetimeType::reset();

  {
    nu::DynamicArray<LifetimeType> buffer;
    LifetimeType l{1, 2};
    buffer.pushBack(l);
  }

  REQUIRE(LifetimeType::creates == 1);
  REQUIRE(LifetimeType::destroys == 2);
  REQUIRE(LifetimeType::copies == 1);
  REQUIRE(LifetimeType::moves == 0);

  LifetimeType::reset();

  {
    nu::DynamicArray<LifetimeType> buffer;
    LifetimeType l{1, 2};
    buffer.pushBack(std::move(l));
  }

  REQUIRE(LifetimeType::creates == 1);
  REQUIRE(LifetimeType::destroys == 2);
  REQUIRE(LifetimeType::copies == 0);
  REQUIRE(LifetimeType::moves == 1);

  LifetimeType::reset();

  {
    nu::DynamicArray<LifetimeType> buffer;
    buffer.emplaceBack(3, 4);
  }

  REQUIRE(LifetimeType::creates == 1);
  REQUIRE(LifetimeType::destroys == 1);
  REQUIRE(LifetimeType::copies == 0);
  REQUIRE(LifetimeType::moves == 0);
}

void dynamicArrayWithoutConst(nu::DynamicArray<U64>& buffer) {
  for (MemSize i = 0; i < buffer.size(); ++i) {
    REQUIRE(buffer[i] == (i + 1) * 10);
  }
}

void dynamicArrayWithConst(const nu::DynamicArray<U64>& buffer) {
  for (MemSize i = 0; i < buffer.size(); ++i) {
    REQUIRE(buffer[i] == (i + 1) * 10);
  }
}

TEST_CASE("can use range-based for loops") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);

  dynamicArrayWithoutConst(buffer);
  dynamicArrayWithConst(buffer);
}

TEST_CASE("remove single element") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);
  buffer.pushBack(40);

  SECTION("from beginning of array") {
    buffer.remove(buffer.begin());

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 20);
    REQUIRE(buffer[1] == 30);
    REQUIRE(buffer[2] == 40);
    REQUIRE(&buffer[3] == buffer.end());
  }

  SECTION("from middle of array") {
    buffer.remove(buffer.begin() + 1);

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 30);
    REQUIRE(buffer[2] == 40);
    REQUIRE(&buffer[3] == buffer.end());
  }

  SECTION("from end of array") {
    buffer.remove(buffer.begin() + 1);

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 30);
    REQUIRE(buffer[2] == 40);
    REQUIRE(&buffer[3] == buffer.end());
  }
}

TEST_CASE("remove calls desctructor") {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  buffer.remove(buffer.begin() + 1);

  REQUIRE(LifetimeType::creates == 4);
  REQUIRE(LifetimeType::destroys == 1);
  REQUIRE(LifetimeType::copies == 0);
}

TEST_CASE("remove range of elements") {
  nu::DynamicArray<U64> buffer;
  buffer.pushBack(10);
  buffer.pushBack(20);
  buffer.pushBack(30);
  buffer.pushBack(40);
  buffer.pushBack(50);
  buffer.pushBack(60);

  SECTION("remove 1 element from the beginning of the array") {
    buffer.remove(buffer.begin(), buffer.begin() + 1);

    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer[0] == 20);
    REQUIRE(buffer[1] == 30);
    REQUIRE(buffer[2] == 40);
    REQUIRE(buffer[3] == 50);
    REQUIRE(buffer[4] == 60);
    REQUIRE(&buffer[5] == buffer.end());
  }

  SECTION("remove 3 elements from the beginning of the array") {
    buffer.remove(buffer.begin(), buffer.begin() + 3);

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 40);
    REQUIRE(buffer[1] == 50);
    REQUIRE(buffer[2] == 60);
    REQUIRE(&buffer[3] == buffer.end());
  }

  SECTION("remove 1 element from the middle of the array") {
    buffer.remove(buffer.begin() + 2, buffer.begin() + 3);

    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 20);
    REQUIRE(buffer[2] == 40);
    REQUIRE(buffer[3] == 50);
    REQUIRE(buffer[4] == 60);
    REQUIRE(&buffer[5] == buffer.end());
  }

  SECTION("remove 3 elements from the middle of the array") {
    buffer.remove(buffer.begin() + 2, buffer.begin() + 5);

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 20);
    REQUIRE(buffer[2] == 60);
    REQUIRE(&buffer[3] == buffer.end());
  }

  SECTION("remove 1 element from the end of the array") {
    buffer.remove(buffer.begin() + 5, buffer.end());

    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 20);
    REQUIRE(buffer[2] == 30);
    REQUIRE(buffer[3] == 40);
    REQUIRE(buffer[4] == 50);
    REQUIRE(&buffer[5] == buffer.end());
  }

  SECTION("remove 3 elements from the middle of the array") {
    buffer.remove(buffer.begin() + 3, buffer.end());

    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer[0] == 10);
    REQUIRE(buffer[1] == 20);
    REQUIRE(buffer[2] == 30);
    REQUIRE(&buffer[3] == buffer.end());
  }
}

TEST_CASE("remove range calls destructors") {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  buffer.remove(buffer.begin() + 1, buffer.begin() + 3);

  REQUIRE(LifetimeType::creates == 4);
  REQUIRE(LifetimeType::destroys == 2);
  REQUIRE(LifetimeType::copies == 0);
}

TEST_CASE("clears elements and calls destructors") {
  LifetimeType::reset();

  nu::DynamicArray<LifetimeType> buffer;
  buffer.emplaceBack(1, 2);
  buffer.emplaceBack(3, 4);
  buffer.emplaceBack(5, 6);
  buffer.emplaceBack(6, 7);

  REQUIRE(buffer.size() == 4);

  buffer.clear();

  REQUIRE(buffer.size() == 0);

  REQUIRE(LifetimeType::creates == 4);
  REQUIRE(LifetimeType::destroys == 4);
  REQUIRE(LifetimeType::copies == 0);
}

}  // namespace nu
