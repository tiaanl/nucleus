
#ifndef NUCLEUS_UTILS_C_STRING_H_
#define NUCLEUS_UTILS_C_STRING_H_

#include "nucleus/Types.h"

namespace nu {

static inline I32 lengthOf(const char* text) {
  I32 l = 0;
  for (; *text; ++text, ++l) {
  }
  return l;
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_C_STRING_H_
