#include "nucleus/streams/utils.h"

namespace nu {

DynamicArray<U8> readEntireStream(InputStream* inputStream) {
  auto bytesRemaining = inputStream->getBytesRemaining();

  DynamicArray<U8> result;
  result.resize(bytesRemaining);

  inputStream->read(result.data(), bytesRemaining);

  return result;
}

}  // namespace nu
