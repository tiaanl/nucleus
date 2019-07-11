
#include "nucleus/Testing.h"

#include "nucleus/Text/StaticString.h"

namespace nu {

TEST_CASE("can copy static string") {
  StaticString<8> str{"test"};
  auto str2 = str;
  CHECK(str2.compare(str) == 0);
}

TEST_CASE("text is pointing to storage") {
  StaticString<4> str{"testing"};
  CHECK(str.getStorageSize() == 4);
  CHECK(str.getLength() == 3);
}

TEST_CASE("can append text") {
  StaticString<32> str{"testing"};
  str.append(" something");
  CHECK(str.getLength() == 17);
}

}  // namespace nu
