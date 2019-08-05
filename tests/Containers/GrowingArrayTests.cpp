#include "nucleus/Containers/DynamicArray.h"
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

TEST_CASE("using iterators across multiple blocks") {
  GrowingArray<Data, 2> ga;
  ga.append({1, 2});
  ga.append({3, 4});
  ga.append({5, 6});

  CHECK(ga.size() == 3);

  auto it = ga.begin();
  CHECK(it->value1 == 1);
  CHECK(it->value2 == 2);
  ++it;
  CHECK(it->value1 == 3);
  CHECK(it->value2 == 4);
  ++it;
  CHECK(it->value1 == 5);
  CHECK(it->value2 == 6);
}

TEST_CASE("can use range based for") {
  GrowingArray<Data, 2> ga;
  ga.append({1, 2});
  ga.append({3, 4});
  ga.append({5, 6});

  nu::DynamicArray<Data> testData;
  for (auto& data : ga) {
    testData.emplaceBack(data.value1, data.value2);
  }
  CHECK(testData[0].value1 == 1);
  CHECK(testData[0].value2 == 2);
  CHECK(testData[1].value1 == 3);
  CHECK(testData[1].value2 == 4);
  CHECK(testData[2].value1 == 5);
  CHECK(testData[2].value2 == 6);
}

TEST_CASE("append with creator function") {
  GrowingArray<Data, 2> ga;
  ga.append([](Data* storage) {
    storage->value1 = 1;
    storage->value2 = 2;
  });

  CHECK(ga.size() == 1);
  CHECK(ga[0]->value1 == 1);
  CHECK(ga[0]->value2 == 2);
}

}  // namespace nu
