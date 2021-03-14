#include <catch2/catch.hpp>

#include "nucleus/Containers/hash_table.h"

namespace nu {

TEST_CASE("HashTable") {
  SECTION("basic") {
    HashTable<I32> t;

    auto res_1 = t.set(10);
    CHECK(res_1.is_new());
    CHECK(res_1.item() == 10);

    auto res_2 = t.set(10);
    CHECK(!res_2.is_new());
    CHECK(res_2.item() == 10);

    auto res_3 = t.set(20);
    CHECK(res_3.is_new());
    CHECK(res_3.item() == 20);

    CHECK(t.contains(10));
    CHECK(!t.contains(11));

    CHECK(!t.remove(11));
    CHECK(t.remove(20));

    CHECK(t.size() == 1);
  }
}

}  // namespace nu
