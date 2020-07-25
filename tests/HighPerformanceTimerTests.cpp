
#include "nucleus/HighPerformanceTimer.h"

#include "nucleus/Testing.h"

namespace nu {

TEST_CASE("high performance timer doesn't return 0") {
  auto now = getCurrentHighPerformanceTick();
  REQUIRE(now != 0);
}

}  // namespace nu
