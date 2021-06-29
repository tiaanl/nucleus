#pragma once

#include "nucleus/text/dynamic_string.h"

namespace nu {

auto zeroTerminated(StringView source) -> DynamicString;

}  // namespace nu
