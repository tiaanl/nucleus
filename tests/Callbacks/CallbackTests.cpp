
#include "nucleus/Testing.h"

#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Callbacks/Callback.h"

namespace nu {

namespace {

void doNothing() {
  LOG(Info) << "Callback called";
}

void doSomethingWith(I32 i) {
  LOG(Info) << "doSomethingWith(" << i << ")";
}

}  // namespace

TEST_CASE("Callbacks: Default constructed callback is empty") {
  Callback<void()> cb;
  CHECK(cb.isEmpty());
}

TEST_CASE("Callbacks: closure") {
  Callback<void()> cb = bind(&doNothing);
  cb.run();
}

TEST_CASE("Callbacks: with arguments") {
  Callback<void()> cb1 = bind(&doNothing, 10);
  cb1.run();
}

}  // namespace nu
