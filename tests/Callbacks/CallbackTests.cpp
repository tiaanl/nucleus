#include <catch2/catch.hpp>

#include "nucleus/Callbacks/Callback.h"
#include "nucleus/Types.h"

namespace nu {

U32 counter = 0;

auto doSomething() -> void {
  LOG(Info) << "Something was done.";
  ++counter;
}

TEST_CASE("callback can be empty") {
  counter = 0;
  auto bound = bind(&doSomething);
  bound.invoke(1);
  CHECK(counter == 1);
}

}  // namespace nu
