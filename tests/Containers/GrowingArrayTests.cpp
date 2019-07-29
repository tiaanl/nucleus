#include "nucleus/Containers/GrowingArray.h"
#include "nucleus/Testing.h"

namespace nu {

struct Data {
  U32 value1;
  U32 value2;
};

TEST_CASE("growing array can default construct") {
  GrowingArray<Data> ga;
  CHECK(ga.empty());
}

TEST_CASE("growing array allocates a block on first append") {
  GrowingArray<Data> ga;
  ga.append({1, 2});
  CHECK(ga.size() == 1);
  CHECK(ga[0]->value1 == 1);
  CHECK(ga[0]->value2 == 2);
}

}  // namespace nu
