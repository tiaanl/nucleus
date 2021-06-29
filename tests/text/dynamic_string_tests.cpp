
#include <catch2/catch.hpp>

#include "nucleus/text/dynamic_string.h"

namespace nu {

TEST_CASE("DynamicString can be copied") {
  DynamicString str1{"testing"};

  SECTION("copy construction") {
    DynamicString str3{str1};
    CHECK(str3.data() != str1.data());
    CHECK(str3.view().compare(str1.view()) == 0);
  }

  SECTION("copy assignment") {
    DynamicString str2;
    str2 = str1;

    CHECK(str2.data() != str1.data());
    CHECK(str1.view().compare(str2.view()) == 0);
  }
}

TEST_CASE("DynamicString does not allocate a buffer when default constructed") {
  DynamicString str;
  CHECK(str.capacity() == 0);
}

TEST_CASE("DynamicString increases size correctly") {
  SECTION("with c-string") {
    DynamicString str{"Do this test!"};
    CHECK(str.length() == 13);
    CHECK(str.capacity() == 16);  // Minimum allocation size.
  }

  SECTION("with StringView") {
    DynamicString str{StringView{"Do this test!", 10}};
    CHECK(str.length() == 10);
    CHECK(str.capacity() == 16);  // Minimum allocation size.
  }
}

TEST_CASE("DynamicString grows when appending") {
  DynamicString str{"Do this test!"};

  SECTION("with c-string") {
    str.append("some more text");
    CHECK(str.length() == 27);
    CHECK(str.capacity() == 32);
  }

  SECTION("with StringView") {
    str.append(StringView{"some more text"});
    CHECK(str.length() == 27);
    CHECK(str.capacity() == 32);
  }
}

TEST_CASE("DynamicString can erase text") {
  DynamicString str{"abcd"};

  SECTION("erase from beginning") {
    str.erase(0, 2);
    CHECK(str.view().compare("cd") == 0);
  }

  SECTION("erase past the allocated size") {
    str.erase(2, 10);
    CHECK(str.view().compare("ab") == 0);
  }
}

}  // namespace nu
