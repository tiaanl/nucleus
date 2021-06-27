
#include "nucleus/Streams/InputStream.h"

#include <algorithm>

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/byte_order.h"

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
        read(temp.data(), std::min(numberOfBytes, static_cast<SizeType>(kBufferedSizeToSkip)));
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

  if (read(temp, 2) == 2) {
    return static_cast<I16>(little_endian_16(temp));
  }

  return 0;
}

I32 InputStream::readI32() {
  char temp[4];

  if (read(temp, 4) == 4) {
    return static_cast<I32>(little_endian_32(temp));
  }

  return 0;
}

I64 InputStream::readI64() {
  union {
    uint8_t asBytes[8];
    U64 asInt64;
  } n;

  if (read(n.asBytes, 8) == 8) {
    return static_cast<I64>(byte_swap_if_big_endian(n.asInt64));
  }

  return 0;
}

U8 InputStream::readU8() {
  U8 temp = 0;
  read(&temp, 1);
  return temp;
}

U16 InputStream::readU16() {
  U8 temp[2];

  if (read(temp, 2) == 2) {
    return static_cast<U16>(little_endian_16(temp));
  }

  return 0;
}

U32 InputStream::readU32() {
  union {
    U8 asBytes[4];
    U32 asU32;
  } n;

  if (read(n.asBytes, sizeof(n)) == sizeof(n)) {
    return static_cast<U32>(byte_swap_if_big_endian(n.asU32));
  }

  return 0;
}

U64 InputStream::readU64() {
  union {
    U8 asBytes[8];
    U64 asU64;
  } n;

  if (read(n.asBytes, sizeof(n)) == sizeof(n)) {
    return static_cast<U64>(byte_swap_if_big_endian(n.asU64));
  }

  return 0;
}

F32 InputStream::readF32() {
  static_assert(sizeof(I32) == sizeof(float),
                "Size of I32 and float must match for the union to work.");

  union {
    I32 asInt;
    float asFloat;
  } n;
  n.asInt = readI32();

  return n.asFloat;
}

F64 InputStream::readF64() {
  static_assert(sizeof(I64) == sizeof(double),
                "Size of I64 and float must match for the union to work.");

  union {
    I64 asInt;
    double asDouble;
  } n;
  n.asInt = readI64();

  return n.asDouble;
}

InputStream::SizeType InputStream::readUntil(void* destination, SizeType bytesToRead,
                                             U8 predicate) {
  U8* buffer = (U8*)destination;
  SizeType bytesRead = 0;
  for (;;) {
    bytesRead += read(buffer, 1);

    if (*buffer == predicate || bytesRead == bytesToRead) {
      return bytesRead;
    }

    ++buffer;
  }
}

InputStream::InputStream() = default;

}  // namespace nu
