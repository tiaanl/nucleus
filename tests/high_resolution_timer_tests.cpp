
#include <catch2/catch.hpp>

#include "nucleus/high_resolution_timer.h"

namespace nu {

TEST_CASE("high performance timer doesn't return 0") {
  auto now = getTimeInMicroseconds();
  REQUIRE(now != 0);
}

}  // namespace nu
