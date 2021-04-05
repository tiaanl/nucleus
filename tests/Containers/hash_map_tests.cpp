#include <catch2/catch.hpp>

#include "nucleus/Containers/hash_map.h"
#include "nucleus/Text/DynamicString.h"

namespace nu {

TEST_CASE("HashMap") {
  SECTION("basic") {
    HashMap<I32, DynamicString> t;

    auto res_1 = t.set(10, StringView{"test"});
    CHECK(res_1.is_new());
    CHECK(res_1.key() == 10);
    CHECK(res_1.value() == StringView{"test"});

    auto res_2 = t.set(10, StringView("Another"));
    CHECK(!res_2.is_new());
    CHECK(res_2.key() == 10);
    CHECK(res_2.value() == StringView("Another"));

    CHECK(t.contains_key(10));

    CHECK(t.find(10)->value.view() == StringView{"Another"});
  }
}

}  // namespace nu