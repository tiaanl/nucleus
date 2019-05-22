
#include "nucleus/Streams/ConsoleOutputStream.h"

#include <cstdio>

#include "nucleus/MemoryDebug.h"

namespace nu {

OutputStream::SizeType ConsoleOutputStream::write(void* buffer, OutputStream::SizeType size) {
  fprintf(stdout, "%s", static_cast<const char*>(buffer));
  return size;
}

}  // namespace nu
