
#include <catch2/catch.hpp>

#include "nucleus/memory/scoped_ref_ptr.h"
#include "nucleus/ref_counted.h"

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
