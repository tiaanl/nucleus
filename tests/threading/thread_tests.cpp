#include <catch2/catch.hpp>

#include "nucleus/logging.h"
#include "nucleus/threading/thread.h"

namespace nu {

TEST_CASE("Thread") {
  SECTION("basic") {
    LOG(Info) << "main thread";
    auto handle = spawn_thread([]() {
      LOG(Info) << "In thread";
    });
    LOG(Info) << "After thread";
  }

  SECTION("can join non running thread") {
    auto handle = spawn_thread([]() {
      LOG(Info) << "In thread";
    });
    handle.join();
    LOG(Info) << "After thread";
  }
}

}  // namespace nu
