#include <catch2/catch.hpp>

#include "nucleus/Text/utils.h"

namespace nu {

TEST_CASE("Has a zero at the end of the source string.") {
  StringView source{"Hello, World!"};
  auto result = zeroTerminated(source);
  CHECK(result[source.length()] == '\0');
}

}  // namespace nu
