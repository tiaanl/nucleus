
#include "nucleus/Streams/InputStream.h"

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Logging.h"
#include "nucleus/Utils/ByteOrder.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

InputStream::SizeType InputStream::getBytesRemaining() {
  DCHECK(getPosition() < getLength()) << "The position should never go over the length of the stream";

  return getLength() - getPosition();
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

float InputStream::readFloat() {
  static_assert(sizeof(I32) == sizeof(float), "Size of int32 and float must match for the union to work.");

  union {
    I32 asInt;
    float asFloat;
  } n;
  n.asInt = readInt32();

  return n.asFloat;
}

double InputStream::readDouble() {
  static_assert(sizeof(I64) == sizeof(double), "Size of int64 and float must match for the union to work.");

  union {
    I64 asInt;
    double asDouble;
  } n;
  n.asInt = readInt64();

  return n.asDouble;
}

std::string InputStream::readNextLine() {
  nu::DynamicArray<char> buffer;
  buffer.resize(256);

  char* data = buffer.getData();
  SizeType i = 0;

  while ((data[i] = readByte()) != 0) {
    if (data[i] == '\n')
      break;

    if (data[i] == '\r') {
      const I64 lastPos = getPosition();

      if (readByte() != '\n')
        setPosition(lastPos);

      break;
    }

    if (++i >= buffer.getSize()) {
      buffer.resize(buffer.getSize() + 512);
      data = buffer.getData();
    }
  }

  return std::string(buffer.getData(), i);
}

std::string InputStream::ReadString() {
  nu::DynamicArray<char> buffer;
  buffer.resize(256);

  char* data = buffer.getData();
  SizeType i = 0;

  while ((data[i] = readByte()) != 0) {
    if (++i >= buffer.getSize()) {
      buffer.resize(buffer.getSize() + 512);
      data = buffer.getData();
    }
  }

  return std::string(data, i);
}

void InputStream::skipNextBytes(SizeType numBytesToSkip) {
  enum { BUFFERED_SIZE_TO_SKIP = 16384 };

  const SizeType skipBufferSize = std::min(numBytesToSkip, static_cast<SizeType>(BUFFERED_SIZE_TO_SKIP));

  nu::DynamicArray<I8> temp;
  temp.resize(skipBufferSize);

  while (numBytesToSkip != 0 && !isExhausted()) {
    numBytesToSkip -= read(temp.getData(), std::min(numBytesToSkip, static_cast<SizeType>(BUFFERED_SIZE_TO_SKIP)));
  }
}

InputStream::InputStream() = default;

}  // namespace nu
