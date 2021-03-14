#include <catch2/catch.hpp>

#include "nucleus/Containers/hash_table.h"

namespace nu {

TEST_CASE("HashTable") {
  SECTION("basic") {
    HashTable<I32> t;

    t.set(10);
    t.set(20);

    CHECK(t.contains(10));
    CHECK(!t.contains(11));

    CHECK(!t.remove(11));
    CHECK(t.remove(20));

    CHECK(t.size() == 1);
  }
}

}  // namespace nu
