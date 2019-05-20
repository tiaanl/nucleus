
#include "nucleus/Parser/XMLParser.h"
#include "nucleus/Testing.h"

namespace nu {

TEST_CASE("Invalid document") {
  auto document = parseXMLDocument("test");
  CHECK(!document.isValid());
}

TEST_CASE("Read tag line") {
  auto document = parseXMLDocument("<?xml version=\"1\"?>");
  CHECK(document.isValid());
}

}  // namespace nu
