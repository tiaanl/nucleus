#ifndef NUCLEUS_TEXT_UTILS_H_
#define NUCLEUS_TEXT_UTILS_H_

#include "nucleus/Text/DynamicString.h"

namespace nu {

auto zeroTerminated(StringView source) -> DynamicString;

}  // namespace nu

#endif  // NUCLEUS_TEXT_UTILS_H_
