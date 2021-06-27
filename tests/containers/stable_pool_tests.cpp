#include <nucleus/testing/lifetime_tracker.h>

#include <catch2/catch.hpp>

#include "nucleus/Containers/stable_pool.h"

namespace nu {

using namespace testing;

TEST_CASE("StablePool") {
  SECTION("basic") {
    StablePool<LifetimeTracker, 8> sp;

    auto* a = sp.construct(10, 20);
    CHECK(sp.size() == 1);
    CHECK(sp.capacity() > 0);
    REQUIRE(a);
    CHECK(a->a() == 10);
    CHECK(a->b() == 20);

    auto* b = sp.insert({11, 21});
    CHECK(sp.size() == 2);
    CHECK(sp.capacity() > 0);
    REQUIRE(b);
    CHECK(b->a() == 11);
    CHECK(b->b() == 21);

    CHECK(sp.remove(b));
  }
}

}  // namespace nu
