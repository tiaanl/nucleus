
#include "nucleus/Testing.h"
#include "nucleus/Text/StaticString.h"

using Catch::Matchers::Equals;

namespace nu {

TEST_CASE("construct StaticString") {
  SECTION("default") {
    StaticString<64> str;

    CHECK(str.length() == 0);
    CHECK(str.capacity() == 64);
  }

  SECTION("from StringView") {
    auto src = StringView{"test"};
    StaticString<64> str{src};

    CHECK(str.length() == 4);
    CHECK(std::strncmp(str.data(), "test", str.length()) == 0);
  }
}

TEST_CASE("can copy static string") {
  StaticString<8> str{"test"};
  auto str2 = str;
  CHECK(str2.view().compare(str.view()) == 0);
}

TEST_CASE("text is pointing to storage") {
  StaticString<4> str{"testing"};
  CHECK(str.capacity() == 4);
  CHECK(str.length() == 4);
}

TEST_CASE("can append text") {
  StaticString<32> str{"testing"};
  str.append(" something");
  CHECK(str.length() == 17);
}

}  // namespace nu
