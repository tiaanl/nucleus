
#include "nucleus/Streams/InputStream.h"

#include "nucleus/ByteOrder.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Logging.h"

#include <algorithm>

#include "nucleus/MemoryDebug.h"

namespace nu {

InputStream::SizeType InputStream::getBytesRemaining() {
  DCHECK(getPosition() <= getLength())
      << "The position should never go over the length of the stream";

  return getLength() - getPosition();
}

void InputStream::skipNextBytes(SizeType numBytesToSkip) {
  constexpr SizeType kBufferedSizeToSkip = 16384;

  const SizeType skipBufferSize =
      std::min(numBytesToSkip, static_cast<SizeType>(kBufferedSizeToSkip));

  nu::DynamicArray<I8> temp;
  temp.resize(skipBufferSize);

  while (numBytesToSkip != 0 && !isExhausted()) {
    numBytesToSkip -=
        read(temp.getData(), std::min(numBytesToSkip, static_cast<SizeType>(kBufferedSizeToSkip)));
  }
}

uint8_t InputStream::readByte() {
  char temp = 0;
  read(&temp, 1);
  return temp;
}

bool InputStream::readBool() {
  return readByte() != 0;
}

I16 InputStream::readInt16() {
  char temp[2];

  if (read(temp, 2) == 2)
    return static_cast<I16>(ByteOrder::littleEndianInt16(temp));

  return 0;
}

I32 InputStream::readInt32() {
  char temp[4];

  if (read(temp, 4) == 4)
    return static_cast<I32>(ByteOrder::littleEndianInt32(temp));

  return 0;
}

I64 InputStream::readInt64() {
  union {
    uint8_t asBytes[8];
    U64 asInt64;
  } n;

  if (read(n.asBytes, 8) == 8)
    return static_cast<I64>(ByteOrder::swapIfBigEndian(n.asInt64));

  return 0;
}

F32 InputStream::readFloat32() {
  static_assert(sizeof(I32) == sizeof(float),
                "Size of int32 and float must match for the union to work.");

  union {
    I32 asInt;
    float asFloat;
  } n;
  n.asInt = readInt32();

  return n.asFloat;
}

F64 InputStream::readFloat64() {
  static_assert(sizeof(I64) == sizeof(double),
                "Size of int64 and float must match for the union to work.");

  union {
    I64 asInt;
    double asDouble;
  } n;
  n.asInt = readInt64();

  return n.asDouble;
}

InputStream::InputStream() = default;

}  // namespace nu
