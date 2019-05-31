
#include "nucleus/Testing.h"

#include "nucleus/Text/StaticString.h"

namespace nu {

TEST_CASE("Basic static string") {
  StaticString<4> s{"testing"};
  CHECK(StringView{"testing"}.compare(s.getData()) == 0);
}

}  // namespace nu
