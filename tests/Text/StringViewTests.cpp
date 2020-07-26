
#include <catch2/catch.hpp>

#include "nucleus/Text/StringView.h"

namespace nu {

namespace {

const char* kTestString = "This is a test string!";

}  // namespace

TEST_CASE("Construct a StringView", "[StringView]") {
  const char* text = "test";

  auto s1 = StringView{text};
  CHECK(s1.length() == 4);

  auto s2 = StringView{text, 3};
  CHECK(s2.length() == 3);

  // Inline syntax.
  CHECK(StringView(text).length() == 4);
}

TEST_CASE("StringView::subString") {
  StringView s{"Some test string"};

  SECTION("position and length is within the bounds") {
    auto r = s.subString(5, 4);

    CHECK(r == "test");
    CHECK(r.length() == 4);
  }

  SECTION("length is out of bounds") {
    auto r = s.subString(5, 20);

    CHECK(r == "test string");
    CHECK(r.length() == 11);
  }

  SECTION("position is out of bounds") {
    auto r = s.subString(20, 20);

    CHECK(r == "");
    CHECK(r.length() == 0);
  }
}

TEST_CASE("StringView findFirstOf") {
  StringView str{kTestString};

  auto pos1 = str.findFirstOf('s');
  CHECK(pos1 == 3);

  auto pos2 = str.findFirstOf('z');
  CHECK(pos2 == StringView::npos);
}

TEST_CASE("StringView findFirstOfAny") {
  StringView str{kTestString};

  auto pos1 = str.findFirstOfAny("sa");
  CHECK(pos1 == 3);

  auto pos2 = str.findFirstOfAny("gs");
  CHECK(pos2 == 3);

  auto pos3 = str.findFirstOfAny("z");
  CHECK(pos3 == StringView::npos);
}

TEST_CASE("StringView findLastOfAny") {
  StringView str{kTestString};

  auto pos1 = str.findLastOfAny("sa");
  CHECK(pos1 == 15);

  auto pos2 = str.findLastOfAny("gs");
  CHECK(pos2 == 20);

  auto pos3 = str.findLastOfAny("z");
  CHECK(pos3 == StringView::npos);
}

}  // namespace nu
