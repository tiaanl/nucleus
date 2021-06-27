
#include <catch2/catch.hpp>

#include "nucleus/Memory/scoped_ptr.h"

namespace nu {

namespace {

struct S {
  static I32 s_constructed;
  static I32 s_destructed;

  static void reset() {
    s_constructed = 0;
    s_destructed = 0;
  }

  int i;

  S() : i(0) {
    ++s_constructed;
  }

  explicit S(I32 i_) : i(i_) {
    ++s_constructed;
  }

  ~S() {
    ++s_destructed;
  }
};

// static
I32 S::s_constructed = 0;
I32 S::s_destructed = 0;

}  // namespace

TEST_CASE("default initialized pointers is owning a null") {
  S::reset();

  ScopedPtr<S> p1;
  REQUIRE(p1.get() == nullptr);

  const ScopedPtr<S> p2;
  REQUIRE(p2.get() == nullptr);
}

TEST_CASE("can own a pointer") {
  S::reset();

  {
    ScopedPtr<S> p1{new S{10}};
    REQUIRE(p1.get()->i == 10);
  }

  REQUIRE(S::s_constructed == 1);
  REQUIRE(S::s_destructed == 1);
}

TEST_CASE("reset") {
  S::reset();

  SECTION("can reset from a pointer") {
    ScopedPtr<S> p1;
    REQUIRE(p1.get() == nullptr);

    p1.reset(new S{20});
    REQUIRE(p1.get()->i == 20);

    p1.reset();
    REQUIRE(p1.get() == nullptr);

    REQUIRE(S::s_constructed == 1);
    REQUIRE(S::s_destructed == 1);
  }
}

TEST_CASE("move") {
  S::reset();

  SECTION("can be moved by construction") {
    {
      ScopedPtr<S> p1{new S{20}};
      ScopedPtr<S> p2{std::move(p1)};
      REQUIRE(p2.get()->i == 20);
    }

    REQUIRE(S::s_constructed == 1);
    REQUIRE(S::s_destructed == 1);
  }
}

}  // namespace nu
