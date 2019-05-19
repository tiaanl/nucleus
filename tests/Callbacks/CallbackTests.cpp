
#include "nucleus/Testing.h"

#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Callbacks/Callback.h"

namespace nu {

static U32 calledNothing = 0;

void nothing() {
  ++calledNothing;
}

TEST_CASE("Basic callback tests") {
  Closure c = bind(&nothing);
  c.run();
  CHECK(calledNothing == 1);
}

}  // namespace nu
