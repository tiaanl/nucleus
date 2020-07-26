
#include <catch2/catch.hpp>

#include "nucleus/Memory/ScopedRefPtr.h"
#include "nucleus/RefCounted.h"

namespace nu {

template <typename T>
struct NoOpRefCountedTraits {
  static void destruct(const T*) {}
};

class Dummy : public RefCounted<Dummy, NoOpRefCountedTraits<Dummy>> {};

TEST_CASE("Increases and decreases reference count") {
  Dummy d;
  {
    ScopedRefPtr<Dummy> s{&d};
    CHECK(d.hasOneRef());
  }
  CHECK(!d.hasOneRef());
}

}  // namespace nu
