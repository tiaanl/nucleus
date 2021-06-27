
#include "nucleus/Streams/console_output_stream.h"

#include <cstdio>

namespace nu {

OutputStream::SizeType ConsoleOutputStream::write(const void *buffer, OutputStream::SizeType size) {
  std::fwrite(buffer, 1, size, stdout);
  return size;
}

}  // namespace nu
