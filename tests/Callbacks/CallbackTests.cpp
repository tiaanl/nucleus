
#include "nucleus/Testing.h"

#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Callbacks/Callback.h"

namespace nu {

namespace {

void doNothing() {}

}  // namespace

TEST_CASE("Callbacks: Default constructed callback is empty") {
  // Callback<void()> cb;
  // CHECK(cb.isEmpty());
}

TEST_CASE("Callbacks: ...") {
  // Callback<void()> cb = bind(&doNothing);
}

}  // namespace nu
