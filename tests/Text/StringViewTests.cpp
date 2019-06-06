
#include "nucleus/Testing.h"
#include "nucleus/Text/StringView.h"

namespace nu {

namespace {

const char* kTestString = "This is a test string!";

}  // namespace

TEST_CASE("Construct a StringView") {
  const char* text = "test";

  auto s1 = StringView{text};
  CHECK(s1.getLength() == 4);

  auto s2 = StringView{text, 3};
  CHECK(s2.getLength() == 3);

  // Inline syntax.
  CHECK(StringView(text).getLength() == 4);
}

TEST_CASE("StringView findFirstOf") {
  StringView str{kTestString};

  auto pos1 = str.findFirstOf('s');
  CHECK(pos1 == 3);

  auto pos2 = str.findFirstOf('z');
  CHECK(pos2 == StringView::kInvalidPosition);
}

TEST_CASE("StringView findFirstOfAny") {
  StringView str{kTestString};

  auto pos1 = str.findFirstOfAny("sa");
  CHECK(pos1 == 3);

  auto pos2 = str.findFirstOfAny("gs");
  CHECK(pos2 == 3);

  auto pos3 = str.findFirstOfAny("z");
  CHECK(pos3 == StringView::kInvalidPosition);
}

TEST_CASE("StringView findLastOfAny") {
  StringView str{kTestString};

  auto pos1 = str.findLastOfAny("sa");
  CHECK(pos1 == 15);

  auto pos2 = str.findLastOfAny("gs");
  CHECK(pos2 == 20);

  auto pos3 = str.findLastOfAny("z");
  CHECK(pos3 == StringView::kInvalidPosition);
}

}  // namespace nu
