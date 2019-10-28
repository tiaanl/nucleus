
#include "nucleus/Testing.h"

#include "nucleus/Text/StaticString.h"

using Catch::Matchers::Equals;

namespace nu {

TEST_CASE("construct StaticString") {
  SECTION("default") {
    StaticString<64> str;

    CHECK(str.getLength() == 0);
    CHECK(str.getStorageSize() == 64);
  }

  SECTION("from StringView") {
    auto src = StringView{"test"};
    StaticString<64> str{src};

    CHECK(str.getLength() == 4);
    CHECK(std::strncmp(str.getData(), "test", str.getLength()) == 0);
  }
}

TEST_CASE("can copy static string") {
  StaticString<8> str{"test"};
  auto str2 = str;
  CHECK(str2.compare(str) == 0);
}

TEST_CASE("text is pointing to storage") {
  StaticString<4> str{"testing"};
  CHECK(str.getStorageSize() == 4);
  CHECK(str.getLength() == 4);
}

TEST_CASE("can append text") {
  StaticString<32> str{"testing"};
  str.append(" something");
  CHECK(str.getLength() == 17);
}

}  // namespace nu
