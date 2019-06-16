#ifndef NUCLEUS_STREAMS_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_OUTPUT_STREAM_H_

#include "nucleus/Text/StringView.h"

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
  void writeI8(I8 data);
  void writeI16(I16 data);
  void writeI32(I32 data);
  void writeI64(I64 data);
  void writeF32(F32 data);
  void writeF64(F64 data);

  void writeString(const StringView& data);

protected:
  explicit OutputStream(OutputStreamMode mode) : m_mode{mode} {}

  OutputStreamMode m_mode = Binary;
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
  output.writeI64(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, F32 value) {
  output.writeF32(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, F64 value) {
  output.writeF64(value);
  return output;
}

inline OutputStream& operator<<(OutputStream& output, const StringView& value) {
  output.writeString(value);
  return output;
}

}  // namespace nu

#endif  // NUCLEUS_STREAMS_OUTPUT_STREAM_H_
