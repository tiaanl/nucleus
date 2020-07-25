
#include "nucleus/Testing.h"
#include "nucleus/Text/String.h"

namespace nu {

TEST_CASE() {
  String s;
  String s2;
}

TEST_CASE("can copy a string") {
  {
    String a{"Hello, World!"};
    String b = a;
  }
}

}  // namespace nu
