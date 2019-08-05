
#include "nucleus/Streams/ConsoleOutputStream.h"

#include <cstdio>

namespace nu {

OutputStream::SizeType ConsoleOutputStream::write(void* buffer, OutputStream::SizeType size) {
  std::fwrite(buffer, 1, size, stdout);
  return size;
}

}  // namespace nu
