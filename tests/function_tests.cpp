#include <catch2/catch.hpp>

#include "nucleus/function.h"

namespace nu {

void do_stuff(int k) {
  LOG(Info) << "Doing static stuff " << k;
}

TEST_CASE("Function") {
  SECTION("basic") {
    int a = 3;
    Function<void(int)> f = [a](int k) {
      LOG(Info) << "Testing stuff! " << a << " " << k;
    };
    f(10);

    f = do_stuff;
    f(10);

    auto g = std::move(f);
    g(10);
  }
}

}  // namespace nu
