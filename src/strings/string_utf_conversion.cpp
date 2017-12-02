
#include "nucleus/strings/string_utf_conversion.h"

#include "nucleus/logging.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

std::u16string ASCIIToUTF16(const std::string& ascii) {
#if 0
  DCHECK(isStringASCII(ascii)) << ascii;
#endif
  return std::u16string(ascii.begin(), ascii.end());
}

std::string UTF16ToASCII(const std::u16string& utf16) {
#if 0
  DCHECK(isStringASCII(utf16)) << UTF16ToUTF8(utf16);
#endif
  return std::string(utf16.begin(), utf16.end());
}

}  // namespace nu
