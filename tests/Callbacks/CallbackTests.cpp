#include <catch2/catch.hpp>

#include "nucleus/Callbacks/Callback.h"
#include "nucleus/Text/StringView.h"

namespace nu {

void addFive(I32 x) {
  LOG(Info) << "addFive(" << x << ")";
}

TEST_CASE("callback can be empty") {
  // auto boundState1 = BoundState::create(&addFive, 10);

  auto p = bind(&addFive);
  p.run(10);
}

}  // namespace nu
