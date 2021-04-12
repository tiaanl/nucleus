#include <catch2/catch.hpp>

#include "nucleus/message_loop/message_loop.h"

namespace nu {

TEST_CASE("MessageLoop") {
  MessageLoop ml;

  SECTION("run until idle") {
    // This run loop should not hang!!
    ml.run_until_idle();
  }

  SECTION("run until quit task") {
    ml.post_quit();
    // This run loop should not hang!!
    ml.run();
  }

  SECTION("executes tasks") {
    U32 value = 10;
    ml.post_task([&value]() {
      value += 1;
    });
    ml.run_until_idle();
    REQUIRE(value == 11);
  }

  SECTION("executes chain until it quits") {
    U32 value = 0;
    ml.post_task([p = &ml, v = &value]() {
      *v = 1;
      p->post_task([p, v]() {
        *v = 2;
        p->post_quit();
      });
    });
    ml.run();
    REQUIRE(value == 2);
  }
}

}  // namespace nu
