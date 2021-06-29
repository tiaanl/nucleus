
#include <catch2/catch.hpp>

#include "nucleus/streams/console_output_stream.h"

namespace nu {

TEST_CASE("Can output to the console") {
  ConsoleOutputStream consoleOutputStream;
  consoleOutputStream << 65 << " and some text\n";
}

}  // namespace nu
