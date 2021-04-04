#include <catch2/catch.hpp>

#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Types.h"
#include "nucleus/scoped_thread_local_ptr.h"

namespace nu {

TEST_CASE("ScopedThreadLocalPtr") {
  SECTION("basic") {
    auto p = ScopedThreadLocalPtr<I32>(new I32(10));
    REQUIRE(*p.get() == 10);
    p.reset(new I32(20));
    REQUIRE(*p.get() == 20);
  }
}

}  // namespace nu
