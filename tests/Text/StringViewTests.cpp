
#include "nucleus/Testing.h"
#include "nucleus/Text/StringView.h"

namespace nu {

TEST_CASE("Construct a StringView") {
  const char* text = "test";

  auto s1 = StringView{text};
  CHECK(s1.getLength() == 4);

  auto s2 = StringView{text, 3};
  CHECK(s2.getLength() == 3);

  // Inline syntax.
  CHECK(StringView(text).getLength() == 4);
}

}  // namespace nu
