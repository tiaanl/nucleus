#include <catch2/catch.hpp>

#include "nucleus/containers/static_array.h"

namespace nu {

struct POD {
  I32 a;
  I32 b;
};

class Object {
public:
  Object(I32 a, I32 b) : m_a{a}, m_b{b} {}

  Object(const Object& other) = default;

  Object(Object&& other) noexcept : m_a{other.m_a}, m_b{other.m_b} {
    other.m_a = 0;
    other.m_b = 0;
  }

  ~Object() = default;

  auto operator=(const Object& other) -> Object& = default;

  auto operator=(Object&& other) noexcept -> Object& {
    m_a = other.m_a;
    m_b = other.m_b;

    other.m_a = 0;
    other.m_b = 0;

    return *this;
  }

  auto a() const -> const I32& {
    return m_a;
  }

  auto b() const -> const I32& {
    return m_b;
  }

private:
  I32 m_a;
  I32 m_b;
};

TEST_CASE("StaticArray construction") {
  SECTION("builtin type") {
    StaticArray<I32, 16> data{};

    CHECK(data.size() == 16);
    CHECK(data.capacity() == 16);
    CHECK(!data.empty());
  }

  SECTION("trivial constructor") {
    StaticArray<POD, 16> data{};

    CHECK(data.size() == 16);
    CHECK(data.capacity() == 16);
    CHECK(!data.empty());
  }

  SECTION("non-trivial constructor") {
    StaticArray<Object, 4> data{
        Object{10, 20},
        Object{30, 40},
        Object{50, 60},
        Object{70, 80},
    };

    CHECK(data.size() == 4);
    CHECK(data.capacity() == 4);
    CHECK(!data.empty());
  }
}

TEST_CASE("StaticArray copy") {
  StaticArray<I32, 4> data = {10, 20, 30, 40};

  CHECK(data.size() == 4);
  CHECK(data.capacity() == 4);
  CHECK(!data.empty());
  CHECK(data[0] == 10);
  CHECK(data[1] == 20);
  CHECK(data[2] == 30);
  CHECK(data[3] == 40);

  auto data2 = data;

  CHECK(data2.size() == 4);
  CHECK(data2.capacity() == 4);
  CHECK(!data2.empty());
  CHECK(data2[0] == 10);
  CHECK(data2[1] == 20);
  CHECK(data2[2] == 30);
  CHECK(data2[3] == 40);
}

TEST_CASE("StaticArray move") {
  StaticArray<Object, 4> data = {
      Object{10, 20},
      Object{30, 40},
      Object{50, 60},
      Object{70, 80},
  };

  CHECK(data.size() == 4);
  CHECK(data.capacity() == 4);
  CHECK(!data.empty());
  CHECK(data[0].a() == 10);
  CHECK(data[1].a() == 30);
  CHECK(data[2].a() == 50);
  CHECK(data[3].a() == 70);

  auto data2 = std::move(data);

  CHECK(data2.size() == 4);
  CHECK(data2.capacity() == 4);
  CHECK(!data2.empty());
  CHECK(data2[0].a() == 10);
  CHECK(data2[1].a() == 30);
  CHECK(data2[2].a() == 50);
  CHECK(data2[3].a() == 70);
}

}  // namespace nu
