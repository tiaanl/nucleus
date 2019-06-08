
#include "nucleus/Streams/InputStream.h"

#include "nucleus/ByteOrder.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Logging.h"

#include <algorithm>

#include "nucleus/MemoryDebug.h"

namespace nu {

InputStream::SizeType InputStream::getBytesRemaining() {
  DCHECK(getPosition() <= getSize())
      << "The position should never go over the length of the stream";

  return getSize() - getPosition();
}

void InputStream::skip(SizeType numberOfBytes) {
  constexpr SizeType kBufferedSizeToSkip = 16384;

  const SizeType skipBufferSize =
      std::min(numberOfBytes, static_cast<SizeType>(kBufferedSizeToSkip));

  nu::DynamicArray<I8> temp;
  temp.resize(skipBufferSize);

  while (numberOfBytes != 0 && !isExhausted()) {
    numberOfBytes -=
        read(temp.getData(), std::min(numberOfBytes, static_cast<SizeType>(kBufferedSizeToSkip)));
  }
}

bool InputStream::readBool() {
  return readI8() != 0;
}

I8 InputStream::readI8() {
  char temp = 0;
  read(&temp, 1);
  return temp;
}

I16 InputStream::readI16() {
  char temp[2];

  if (read(temp, 2) == 2)
    return static_cast<I16>(ByteOrder::littleEndianInt16(temp));

  return 0;
}

I32 InputStream::readI32() {
  char temp[4];

  if (read(temp, 4) == 4)
    return static_cast<I32>(ByteOrder::littleEndianInt32(temp));

  return 0;
}

I64 InputStream::readI64() {
  union {
    uint8_t asBytes[8];
    U64 asInt64;
  } n;

  if (read(n.asBytes, 8) == 8)
    return static_cast<I64>(ByteOrder::swapIfBigEndian(n.asInt64));

  return 0;
}

F32 InputStream::readF32() {
  static_assert(sizeof(I32) == sizeof(float),
                "Size of int32 and float must match for the union to work.");

  union {
    I32 asInt;
    float asFloat;
  } n;
  n.asInt = readI32();

  return n.asFloat;
}

F64 InputStream::readF64() {
  static_assert(sizeof(I64) == sizeof(double),
                "Size of int64 and float must match for the union to work.");

  union {
    I64 asInt;
    double asDouble;
  } n;
  n.asInt = readI64();

  return n.asDouble;
}

InputStream::InputStream() = default;

}  // namespace nu
