
#include <catch2/catch.hpp>

#include "nucleus/Logging.h"

namespace nu {

TEST_CASE() {
  LOG(Info) << "testing";
  DLOG(Warning) << "this only logs in debug mode";

  /*
  int a = 10;
  DCHECK(a == 11) << "a should be 10";
  DCHECK(a == 10) << "this should not show";
  */
}

}  // namespace nu
