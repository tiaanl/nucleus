
#ifndef NUCLEUS_TEXT_STRING_H_
#define NUCLEUS_TEXT_STRING_H_

#include <algorithm>
#include <ostream>

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Utils/CString.h"
#include "nucleus/Utils/Copy.h"

namespace nu {

class String {
public:
  using CharType = I8;
  using SizeType = I32;

  static constexpr SizeType npos = -1;

  explicit String(Allocator* allocator = getDefaultAllocator())
    : m_allocator(allocator), m_data(nullptr), m_length(0), m_allocated(0) {}

  String(const String& other, Allocator* allocator = getDefaultAllocator())
    : String(other.m_data, other.getLength(), allocator) {}

  // Construct a `String` from a c-string.
  String(const char* text, SizeType length = npos, Allocator* allocator = getDefaultAllocator())
    : m_allocator(allocator), m_data(nullptr), m_length(0), m_allocated(0) {
    SizeType textLength = (length == npos) ? lengthOf(text) : length;
    ensureAllocated(textLength, false);
    std::copy(text, text + textLength, m_data);
    m_length = textLength;
    m_data[m_length] = 0;
  }

  // Destruct the string, freeing any memory we might have allocated.
  ~String() {
    if (m_data && m_allocated) {
      m_allocator->free(m_data, m_allocated);
    }
  }

  // Copy

  String& operator=(const String& other) {
    ensureAllocated(other.getLength(), false);
    std::copy(other.m_data, other.m_data + other.m_length, m_data);
    m_length = other.m_length;
    m_data[m_length] = 0;

    return *this;
  }

  Allocator* getAllocator() const {
    return m_allocator;
  }

  // Returns the length of the string.
  SizeType getLength() const {
    return m_length;
  }

  const char* getRawBytes() const {
    return m_data;
  }

  CharType& operator[](SizeType index) {
    return m_data[index];
  }

  CharType operator[](SizeType index) const {
    return m_data[index];
  }

  // Returns true if the string contains no characters.
  bool isEmpty() const {
    return m_length == 0;
  }

  SizeType find(CharType ch, SizeType pos = 0, SizeType count = npos) const {
    for (SizeType i = pos; i < pos + count; ++i) {
      if (m_data[i] == ch) {
        return i;
      }
    }
    return npos;
  }

  String sub(SizeType pos, SizeType count = npos) const {
    return String(m_data + pos, count, m_allocator);
  }

  // Mutations

  void resize(SizeType newLength) {
    // Make sure we can fit the new length of string.
    ensureAllocated(newLength, true);

    m_length = newLength;

    // Set the null terminator for in case the new length is less than the old length.
    m_data[m_length] = 0;
  }

  // Append a single character to the string.
  void append(CharType ch) {
    ensureAllocated(m_length + 1, true);
    m_data[m_length++] = ch;
    m_data[m_length] = 0;
  }

  // Append another string to the string.
  void append(const String& other) {
    ensureAllocated(m_length + other.m_length, true);
    std::copy(other.m_data, other.m_data + other.m_length, m_data + m_length);
    m_length += other.m_length;
    m_data[m_length] = 0;
  }

  // Erase `count` characters starting from `pos`.
  void erase(SizeType pos, SizeType count) {
    for (SizeType i = 0; i < m_length - count; ++i) {
      m_data[pos + i] = m_data[pos + count + i];
    }
    m_length -= count;
    m_data[m_length] = 0;
  }

  // Operators

  bool operator==(const String& other) const {
    const char* left = m_data;
    const char* right = other.m_data;
    for (SizeType i = 0; i < m_length; ++i) {
      if (*left++ != *right++) {
        return false;
      }
    }
    return true;
  }

  I32 compare(const String& other) const {
    const char* left = m_data;
    const char* right = other.m_data;

    for (; *left && *right; left++, right++) {
      if (*left < *right) {
        return -1;
      }

      if (*right < *left) {
        return 1;
      }
    }

    return 0;
  }

  // Returns the position of the last occurrence of `ch` in the string.  Returns `npos` if the character is not found.
  SizeType findLastOfChar(CharType ch) const {
    for (SizeType i = m_length - 1; i >= 0; --i) {
      if (m_data[i] == ch) {
        return i;
      }
    }
    return npos;
  }

  // Find the last occurance of any of the characters in `chars` starting from `start` position.
  SizeType findLastOfAnyChar(const String& chars) const {
    for (SizeType i = m_length - 1; i >= 0; --i) {
      SizeType pos = chars.findLastOfChar(m_data[i]);
      if (pos != npos) {
        return i;
      }
    }
    return npos;
  }

private:
  // Ensure that the buffer can hold a string of the specified length.  If `keepOld` is true, then any data currently in
  // the buffer will be there after any allocations.
  void ensureAllocated(I32 lengthOfString, bool keepOld) {
    if (static_cast<SizeType>(m_allocated) < lengthOfString + 1) {
      allocate(lengthOfString + 1, keepOld);
    }
  }

  // Allocate memory to hold a string of the specified length.  If `keepOld` is true, then any data currently in the
  // buffer will be there after any allocations.
  void allocate(USize bytesToAllocate, bool keepOld) {
    // Allocate a new buffer to hold data.
    CharType* newBuffer = static_cast<CharType*>(m_allocator->allocate(bytesToAllocate));

    if (m_data) {
      // Copy the old data if needed.
      if (keepOld) {
        std::copy(m_data, m_data + std::min<USize>(bytesToAllocate, m_length), newBuffer);
        newBuffer[m_length] = 0;
      }

      // Destroy the old buffer.
      m_allocator->free(m_data, m_allocated);
    }

    m_data = newBuffer;
    m_allocated = bytesToAllocate;
  }

  nu::Allocator* m_allocator;
  CharType* m_data;
  SizeType m_length;
  USize m_allocated;
};

inline std::ostream& operator<<(std::ostream& os, const String& s) {
  os << s.getRawBytes();
  return os;
}

}  // namespace nu

#endif  // NUCLEUS_TEXT_STRING_H_
