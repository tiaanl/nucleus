#pragma once

#include <string>

#include "nucleus/macros.h"
#include "nucleus/types.h"

namespace nu {

class InputStream {
  NU_DELETE_COPY_AND_MOVE(InputStream);

public:
  using SizeType = MemSize;

  virtual ~InputStream() = default;

  // Return the total number of bytes available from the beginng of the stream.
  virtual SizeType getSize() = 0;

  // Return the number of bytes available for reading.
  SizeType getBytesRemaining();

  // Return true if the stream has no more data to read.
  virtual bool isExhausted() = 0;

  // Return the offset of the next byte that will be read from the stream.
  virtual SizeType getPosition() = 0;

  // Try to move the current read position of the stream.  The position is an absolute number of
  // bytes from the stream's start.  If the position could not be set, then false is returned.
  virtual bool setPosition(SizeType newPosition) = 0;

  // Read some data from the stream into a memory buffer.
  virtual SizeType read(void* destination, SizeType bytesToRead) = 0;

  // Read some data from the stream into the destination, stopping when `bytesToRead` have been read
  // or the predicate byte has been found.
  SizeType readUntil(void* destination, SizeType bytesToRead, U8 predicate);

  // Read and discard a number of bytes from the stream.
  virtual void skip(SizeType numberOfBytesToSkip);

  // Read a boolean from the stream.  The bool is encoded as a single byte. 0 for false and anything
  // else for true.  If the stream is exhausted while reading the bytes, this will return false.
  bool readBool();

  // Read a byte from the stream.  If the stream is exhausted, then 0 will be returned.
  I8 readI8();

  // Read two bytes from the stream as a little-endian 16-bit value.   If the stream is exhausted,
  // then zero will be returned.
  I16 readI16();

  // Read four bytes from the stream as a little-endian 32-bit value.  If the stream is exhausted,
  // then zero will be returned.
  I32 readI32();

  // Read eight bytes from the stream as a little-endian 64-bit value.  If the stream is exhausted,
  // then 0 will be returned.
  I64 readI64();

  // Read one byte from the stream as a little-endian unsigned 8-bit value.  If the stream is
  // exhausted, then 0 will be returned.
  U8 readU8();

  // Read two bytes from the stream as a little-endian unsigned 16-bit value.  If the stream is
  // exhausted, then 0 will be returned.
  U16 readU16();

  // Read four bytes from the stream as a little-endian unsigned 32-bit value.  If the stream is
  // exhausted, then 0 will be returned.
  U32 readU32();

  // Read eight bytes from the stream as a little-endian unsigned 64-bit value.  If the stream is
  // exhausted, then 0 will be returned.
  U64 readU64();

  // Read four bytes as a little-endian  32-bit floating point value.  If the stream is exhausted,
  // then 0.0f will be returned.
  F32 readF32();

  // Read eight bytes as a little-endian 64-bit floating point value.  If the stream is exhausted,
  // then 0.0 will be returned.
  F64 readF64();

protected:
  InputStream();
};

#define DECLARE_OPERATOR(Type)                                                                     \
  inline InputStream& operator<<(InputStream& inputStream, Type& out) {                            \
    out = inputStream.read##Type();                                                                \
    return inputStream;                                                                            \
  }

DECLARE_OPERATOR(U8)
DECLARE_OPERATOR(U16)
DECLARE_OPERATOR(U32)
DECLARE_OPERATOR(U64)
DECLARE_OPERATOR(I8)
DECLARE_OPERATOR(I16)
DECLARE_OPERATOR(I32)
DECLARE_OPERATOR(I64)
DECLARE_OPERATOR(F32)
DECLARE_OPERATOR(F64)

#undef DECLARE_OPERATOR

}  // namespace nu
