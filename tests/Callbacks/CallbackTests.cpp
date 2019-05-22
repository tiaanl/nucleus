
#include "nucleus/Testing.h"

#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Callbacks/Callback.h"

namespace nu {

static U32 calledNothing = 0;

void nothing(I32 start) {
  calledNothing = start + 1;
}

TEST_CASE("Basic callback tests") {
  {
    Closure c = bind(&nothing, 10);
    c.run();
    CHECK(calledNothing == 11);
  }

  {
    Callback<void(I32)> cb = bind(&nothing);
    cb.run(10);
    CHECK(calledNothing == 11);
  }
}

}  // namespace nu
