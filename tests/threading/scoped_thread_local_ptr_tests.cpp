#include <catch2/catch.hpp>

#include "nucleus/Memory/scoped_ptr.h"
#include "nucleus/threading/scoped_thread_local_ptr.h"
#include "nucleus/threading/thread.h"

namespace nu {

TEST_CASE("ScopedThreadLocalPtr") {
  SECTION("thread local storage on different thread should be different") {
    auto p = ScopedThreadLocalPtr<I32>(new I32(10));

    auto join = spawn_thread([&p]() {
      // Although this is using the same object as the one on the test thread, this one should not
      // be initialized, because we're running on a different thread.
      REQUIRE(p.get() == nullptr);
    });

    REQUIRE(*p.get() == 10);
    p.reset(new I32(20));
    REQUIRE(*p.get() == 20);
  }
}

}  // namespace nu
