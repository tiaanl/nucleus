#include <catch2/catch.hpp>

#include "nucleus/hash.h"

namespace nu {

TEST_CASE("hash basic") {
  auto hash = Hash<I32>::hashed(10);
  CHECK(hash != 10);
}

}  // namespace nu
