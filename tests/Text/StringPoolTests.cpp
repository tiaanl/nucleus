
#include <catch2/catch.hpp>

#include "nucleus/Text/StringPool.h"

namespace nu {

TEST_CASE("Can store a stirngs") {
  StringPool<32, 2> sp;
  CHECK(sp.getCapacity() == 0);

  SECTION("Store string in single space") {
    sp.store("This is a test");
    CHECK(sp.getCapacity() == 32 * 2);
  }

  SECTION("Store string in more than one space") {
    sp.store("This is a long string that will take more than one block.");
    CHECK(sp.getCapacity() == 32 * 2);
  }

  SECTION("Store a string in more than one block") {
    sp.store("We store a string that is more than a single chunk.");
    sp.store("We store a string that is more than a single chunk.");
    CHECK(sp.getCapacity() == 32 * 2 * 2);
  }
}

TEST_CASE("Report error when storing a string that is too long") {
  StringPool<10, 2> sp;
  auto result = sp.store("This is a string longer than 10");
  CHECK(result.length() == 0);
}

}  // namespace nu
