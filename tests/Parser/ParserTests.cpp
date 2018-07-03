
#include "nucleus/Logging.h"
#include "nucleus/Parser/Parser.h"
#include "nucleus/Streams/WrappedMemoryInputStream.h"
#include "nucleus/Testing.h"

#include "nucleus/Memory/RefCountedPtr.h"
#include "nucleus/Memory/ScopedRefPtr.h"

#include <tuple>

namespace nu {

TEST_CASE("Can parse input stream") {
  auto source = R"eof(
    This is some source
    identifierWithNumbers123
    number: 123
    decimal: 3.14
    punctuation `~!@#$%^&*()-_=+[{]}\|;:',<.>/?
    "String literal"
    identifier
    )eof";
  WrappedMemoryInputStream stream(source, std::strlen(source));

  auto ps = Parser::parseStream(&stream);
  for (auto& token : ps) {
    LOG(Info) << token.index << "[" << token.length << "] = " << ps.getTokenSource(token);
  }
}

}  // namespace nu
