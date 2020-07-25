
#include "nucleus/Streams/ConsoleOutputStream.h"
#include "nucleus/Testing.h"

namespace nu {

TEST_CASE("Can output to the console") {
  ConsoleOutputStream consoleOutputStream;
  consoleOutputStream << 65 << " and some text\n";
}

}  // namespace nu
