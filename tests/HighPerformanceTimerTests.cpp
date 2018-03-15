
#include "nucleus/HighPerformanceTimer.h"

#include <thread>

#include "nucleus/Logging.h"
#include "nucleus/Testing.h"

namespace nu {

TEST_CASE("high performance timer doesn't return 0") {
  F64 now = getCurrentHighPerformanceTick();
  REQUIRE(now != 0);
}

TEST_CASE("make sure frequency is in milliseconds") {
  F64 total = 0.0;

  for (int i = 0; i < 100; ++i) {
    F64 first = getCurrentHighPerformanceTick();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    F64 second = getCurrentHighPerformanceTick();

    total += second - first;
  }

  total /= 100.0;

  CHECK(total >= 1.0);
  CHECK(total <= 2.0);
}

}  // namespace nu
