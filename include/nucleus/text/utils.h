#pragma once

#include "nucleus/Text/dynamic_string.h"

namespace nu {

auto zeroTerminated(StringView source) -> DynamicString;

}  // namespace nu
