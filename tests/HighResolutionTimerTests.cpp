
#include <catch2/catch.hpp>

#include "nucleus/HighResolutionTimer.h"

namespace nu {

TEST_CASE("high performance timer doesn't return 0") {
  auto now = getTimeInMicroseconds();
  REQUIRE(now != 0);
}

}  // namespace nu
