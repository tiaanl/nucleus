#ifndef NUCLEUS_STREAMS_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_OUTPUT_STREAM_H_

#include "nucleus/types.h"

namespace nu {

class OutputStream {
public:
  using SizeType = USize;

  virtual ~OutputStream() = default;

  // Write the given buffer to the stream.  Returns the number of bytes written.
  virtual SizeType write(void* buffer, SizeType size) = 0;

  // Flush the stream.  Returns true on success.
  //  virtual bool flush() = 0;

  // Close the stream.
  //  virtual void close() = 0;

  void writeBool(bool data);
  void writeU8(U8 data);
  void writeInt16(I16 data);
  void writeInt32(I32 data);
  void writeInt64(I64 data);
  void writeFloat(F32 data);
  void writeDouble(F64 data);

#if 0
  std::string readNextLine();
  virtual std::string ReadString();
  virtual void skipNextBytes(SizeType numBytesToSkip);
#endif  // 0
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_OUTPUT_STREAM_H_
