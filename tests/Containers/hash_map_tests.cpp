#include <catch2/catch.hpp>

#include "nucleus/Containers/hash_map.h"
#include "nucleus/Text/DynamicString.h"

namespace nu {

TEST_CASE("HashMap") {
  SECTION("basic") {
    HashMap<I32, DynamicString> t;

    CHECK(t.set(10, StringView{"test"}));
    CHECK(!t.set(10, StringView("Another")));

    CHECK(t.contains_key(10));

    CHECK(t.find(10)->value.view() == StringView{"Another"});
  }
}

}  // namespace nu
