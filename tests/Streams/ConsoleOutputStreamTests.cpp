
#include "nucleus/Testing.h"
#include "nucleus/Streams/ConsoleOutputStream.h"

namespace nu {

TEST_CASE("Can output to the console") {
  ConsoleOutputStream cout;
  cout << 65 << " and some text\n";
}

}  // namespace nu
