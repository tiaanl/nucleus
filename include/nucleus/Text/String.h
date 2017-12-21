
#ifndef NUCLEUS_TEXT_STRING_H_
#define NUCLEUS_TEXT_STRING_H_

#include "nucleus/Containers/DynamicArray.h"

namespace nu {

template <typename CharTraits>
class String {
public:
    using CharType = I8;

    String(Allocator* allocator = getDefaultAllocator()) : m_data(allocator) {}

    // Construct a string from a C-style string.
    String(const char* text, USize length) {
        m_data.reserve(length + 1);
        CharTraits::writeCString(m_data.getData(), text, length);
    }

    ~String() {}

private:
    DynamicArray<CharType> m_data;
};

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_H_
