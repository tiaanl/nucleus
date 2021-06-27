
#include <catch2.hpp>

#include "nucleus/Parser/xml_parser.h"

namespace nu {

TEST_CASE("Invalid document") {
  auto document = parseXMLDocument("test");
}

TEST_CASE("Read tag line") {
  auto document = parseXMLDocument("<?xml version=\"1\"?>");
}

}  // namespace nu
