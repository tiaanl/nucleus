
#include "nucleus/Testing.h"

#include "nucleus/Callbacks/Bind.h"
#include "nucleus/Callbacks/Callback.h"

#include "nucleus/Win/WindowsMixin.h"

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
  Callback<void()> cb1 = bind(&doSomethingWith, 10);
  cb1.run();

  Callback<void(int)> cb2 = bind(&doSomethingWith);
  cb2.run(20);
}

}  // namespace nu
