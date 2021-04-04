#include <catch2/catch.hpp>

#include "nucleus/Logging.h"
#include "nucleus/threading/thread.h"

namespace nu {

TEST_CASE("Thread") {
  class SetNewValueTask : public Task {
  public:
    void execute() override {
      LOG(Info) << "In thread";
    }
  };

  SECTION("basic") {
    LOG(Info) << "main thread";
    auto handle = spawn_thread(new SetNewValueTask);
    LOG(Info) << "After thread";
  }

  SECTION("can join non running thread") {
    auto handle = spawn_thread(new SetNewValueTask);
    handle.join();
    LOG(Info) << "After thread";
  }
}

}  // namespace nu
