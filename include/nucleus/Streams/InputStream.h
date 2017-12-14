
#ifndef NUCLEUS_STREAMS_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_INPUT_STREAM_H_

#include <string>

#include "nucleus/Macros.h"
#include "nucleus/Types.h"

namespace nu {

// This is the abstract base class for all types of input streams.
class InputStream {
public:
  using SizeType = USize;

  virtual ~InputStream() {}

  // Return the total number of bytes available from the beginng of the
  // stream.
  virtual SizeType getLength() = 0;

  // Return the number of bytes available for reading.
  SizeType getBytesRemaining();

  // Return true if the stream has no more data to read.
  virtual bool isExhausted() = 0;

  // Return the offset of the next byte that will be read from the stream.
  virtual SizeType getPosition() = 0;

  // Try to move the current read position of the stream.  The position is an
  // absolute number of bytes from the stream's start.  Some streams might not
  // be able to do this, in which case they should do nothing and return
  // false. Others might be able to manage it by resetting themselves and
  // skipping to the correct position, although this is obviously a bit slow.
  virtual bool setPosition(SizeType newPosition) = 0;

  // Read some data from the stream into a memory buffer.
  virtual SizeType read(void* destBuffer, SizeType bytesToRead) = 0;

  // Read a byte from the stream. If the stream is exhausted while reading the
  // bytes, this will return 0.
  U8 readByte();

  // Read a boolean from the stream.  The bool is encoded as a single byte.
  // 0 for false and anything else for true.  If the stream is exhausted while
  // reading the bytes, this will return false.
  bool readBool();

  // Reads two bytes from the stream as a little-endian 16-bit value.  If the
  // next two bytes are byte1 and byte2, this returns:
  // (byte1 | (byte2 << 8))
  // If the stream is exhausted while reading the bytes, this will return
  // zero.
  I16 readInt16();

  // Read four bytes from the stream as a little-endian 32-bit value.  If the
  // next four bytes are byte1 to byte4, this returns:
  // (byte1 | (byte2 << 8) | (byte3 << 16) | (byte4 << 24))
  // If the stream is exhausted while reading the bytes, this will return
  // zero.
  I32 readInt32();

  // Read eight bytes from the stream as a little-endian 64-bit value.  If the
  // next eight bytes are byte1 to byte8, this returns:
  // (byte1 | (byte2 << 8) | (byte3 << 16) | (byte4 << 24) | (byte5 << 32)
  //   | (byte6 << 40) | (byte7 << 48) | (byte8 << 56))
  // If the stream is exhausted while reading the bytes, this will return
  // zero.
  I64 readInt64();

  // Read four bytes as a 32-bit floating point value.  The raw 32-bit
  // encoding of the float is read from the stream as a little-endian int32.
  // If the stream is exhausted while reading the bytes, this will return
  // zero.
  float readFloat();

  // Read eight bytes as a 64-bit floating point value.  The raw 64-bit
  // encoding of the float is read from the stream as a little-endian int64.
  // If the stream is exhausted while reading the bytes, this will return
  // zero.
  double readDouble();

  // Read a UTF-8 string from the stream.  This will read up to the next "\n"
  // or "\r\n" or end-of-stream.  After the call, the stream's position will
  // be left pointing to the next character following the line-feed, but the
  // line-feeds aren't included in the string that is returned.
  std::string readNextLine();

  // Read a zero-terminated UTF-8 string from the stream.  This will read
  // characters from the stream until it hits a null character or
  // end-of-stream.
  virtual std::string ReadString();

  // Read and discard a number of bytes from the stream.  Some input streams
  // might implement this efficiently, but the base class will just keep
  // reading data until the specified number of bytes have been done.
  virtual void skipNextBytes(SizeType numBytesToSkip);

protected:
  InputStream();

private:
  DISALLOW_COPY_AND_ASSIGN(InputStream);
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_INPUT_STREAM_H_
