#ifndef NUCLEUS_STREAMS_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_OUTPUT_STREAM_H_

#include "nucleus/Types.h"

namespace nu {

class OutputStream {
public:
  using SizeType = MemSize;

  enum OutputStreamMode { Binary, Text };

  virtual ~OutputStream() = default;

  // Write the given buffer to the stream.  Returns the number of bytes written.
  virtual SizeType write(void* buffer, SizeType size) = 0;

  void writeBool(bool data);
  void writeU8(U8 data);
  void writeU16(U16 data);
  void writeU32(U32 data);
  void writeU64(U64 data);
  void writeInt16(I16 data);
  void writeInt32(I32 data);
  void writeInt64(I64 data);
  void writeFloat32(F32 data);
  void writeFloat64(F64 data);
  void writeCString(const char* data);

protected:
  explicit OutputStream(OutputStreamMode mode) : m_mode{mode}, m_buffer{} {}

  OutputStreamMode m_mode = Binary;

  // Buffer used to convert values to text.
  I8 m_buffer[64];
};

inline OutputStream& operator<<(OutputStream& output, bool value) {
  output.writeBool(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, U32 value) {
  output.writeU32(value);
  return output;
}

#if 0
inline OutputStream& operator<<(OutputStream& output, U8 value) {
  output.writeU8(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, I16 value) {
  output.writeInt16(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, I32 value) {
  output.writeInt32(value);
  return output;
}
#endif  // 0

inline OutputStream& operator<<(OutputStream& output, I64 value) {
  output.writeInt64(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, F32 value) {
  output.writeFloat32(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, F64 value) {
  output.writeFloat64(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, const char* value) {
  output.writeCString(value);
  return output;
}

}  // namespace nu

#endif  // NUCLEUS_STREAMS_OUTPUT_STREAM_H_
