
#include <catch2/catch.hpp>

#include "nucleus/HighPerformanceTimer.h"

namespace nu {

TEST_CASE("high performance timer doesn't return 0") {
  auto now = getCurrentHighPerformanceTick();
  REQUIRE(now != 0);
}

}  // namespace nu
