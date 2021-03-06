#include "nucleus/text/utils.h"

namespace nu {

auto zeroTerminated(StringView source) -> DynamicString {
  DynamicString result{source};
  result.append('\0');
  return result;
}

}  // namespace nu
