
#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Logging.h"
#include "nucleus/MessageLoop/MessageLoop.h"
#include "nucleus/Testing.h"

namespace nu {

namespace {

void increment(I32* counter) {
  *counter++;
}

}  // namespace

TEST_CASE() {
  // MessageLoop ml;

  // I32 counter = 0;
  // MessageLoop::current()->postTask(bind(&increment, &counter));

  // MessageLoop::current()->runUntilIdle();

  // CHECK(counter == 1);
}

}  // namespace nu
