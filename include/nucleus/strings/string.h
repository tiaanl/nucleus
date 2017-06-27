
#ifndef NUCLEUS_STRINGS_STRING_H_
#define NUCLEUS_STRINGS_STRING_H_

#include "nucleus/types.h"

namespace nu {

class String {
public:
    typedef U8 CharType;
    typedef USize SizeType;

private:
    CharType* begin;
    CharType* end;
};

}  // namespace nu

#endif  // NUCLEUS_STRINGS_STRING_H_
