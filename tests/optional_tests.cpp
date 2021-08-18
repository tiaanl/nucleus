#include <catch2/catch.hpp>

#include "nucleus/optional.hpp"
#include "nucleus/testing/lifetime_tracker.h"

namespace nu {

using testing::LifetimeTracker;

TEST_CASE("Optional") {
  LifetimeTracker::reset();

  SECTION("empty") {
    {
      Optional<LifetimeTracker> o;
      CHECK(!o.has_value());
    }

    CHECK(LifetimeTracker::creates == 0);
    CHECK(LifetimeTracker::copies == 0);
    CHECK(LifetimeTracker::moves == 0);
    CHECK(LifetimeTracker::destroys == 0);
  }

  SECTION("default construct") {
    {
      Optional<LifetimeTracker> o{10, 20};
      REQUIRE(o.has_value());
      CHECK(o.value().a() == 10);
      CHECK(o.value().b() == 20);
    }

    CHECK(LifetimeTracker::creates == 1);
    CHECK(LifetimeTracker::copies == 0);
    CHECK(LifetimeTracker::moves == 0);
    CHECK(LifetimeTracker::destroys == 1);
  }

  SECTION("move construct") {
    {
      Optional<LifetimeTracker> o{LifetimeTracker{30, 40}};
      REQUIRE(o.has_value());
      CHECK(o.value().a() == 30);
      CHECK(o.value().b() == 40);
    }

    CHECK(LifetimeTracker::creates == 1);
    CHECK(LifetimeTracker::copies == 0);
    CHECK(LifetimeTracker::moves == 1);
    CHECK(LifetimeTracker::destroys == 2);
  }

  SECTION("copy construct") {
    {
      LifetimeTracker obj{30, 40};
      Optional<LifetimeTracker> o{obj};
      REQUIRE(o.has_value());
      CHECK(o.value().a() == 30);
      CHECK(o.value().b() == 40);
    }

    CHECK(LifetimeTracker::creates == 1);
    CHECK(LifetimeTracker::copies == 1);
    CHECK(LifetimeTracker::moves == 0);
    CHECK(LifetimeTracker::destroys == 2);
  }

  SECTION("move assignment") {
    {
      Optional<LifetimeTracker> a{30, 40};
      Optional<LifetimeTracker> b;
      b = std::move(a);
      REQUIRE(b.has_value());
      CHECK(b.value().a() == 30);
      CHECK(b.value().b() == 40);
    }

    CHECK(LifetimeTracker::creates == 1);
    CHECK(LifetimeTracker::copies == 0);
    CHECK(LifetimeTracker::moves == 1);
    CHECK(LifetimeTracker::destroys == 1);
  }

  SECTION("copy assignment") {
    {
      Optional<LifetimeTracker> a{30, 40};
      Optional<LifetimeTracker> b;
      b = a;
      REQUIRE(b.has_value());
      CHECK(b.value().a() == 30);
      CHECK(b.value().b() == 40);
    }

    CHECK(LifetimeTracker::creates == 1);
    CHECK(LifetimeTracker::copies == 1);
    CHECK(LifetimeTracker::moves == 0);
    CHECK(LifetimeTracker::destroys == 2);
  }
}

}  // namespace nu
