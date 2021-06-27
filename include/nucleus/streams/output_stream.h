#pragma once

#include <cstring>

#include "nucleus/types.h"

namespace nu {

class OutputStream {
public:
  using SizeType = MemSize;

  enum OutputStreamMode { Binary, Text };

  virtual ~OutputStream() = default;

  // Write the given buffer to the stream.  Returns the number of bytes written.
  virtual SizeType write(const void* buffer, SizeType size) = 0;

  void writeBool(bool value);
  void writeU8(U8 value);
  void writeU16(U16 value);
  void writeU32(U32 value);
  void writeU64(U64 value);
  void writeI8(I8 value);
  void writeI16(I16 value);
  void writeI32(I32 value);
  void writeI64(I64 value);
  void writeF32(F32 value);
  void writeF64(F64 value);

protected:
  explicit OutputStream(OutputStreamMode mode) : m_mode{mode} {}

  OutputStreamMode m_mode = Binary;
};

#define DEFINE_OPERATOR(Type)                                                                      \
  inline OutputStream& operator<<(OutputStream& output, Type value) {                              \
    output.write##Type(value);                                                                     \
    return output;                                                                                 \
  }

DEFINE_OPERATOR(U8)
DEFINE_OPERATOR(U16)
DEFINE_OPERATOR(U32)
DEFINE_OPERATOR(U64)
DEFINE_OPERATOR(I8)
DEFINE_OPERATOR(I16)
DEFINE_OPERATOR(I32)
DEFINE_OPERATOR(I64)
DEFINE_OPERATOR(F32)
DEFINE_OPERATOR(F64)

#undef DEFINE_OPERATOR

inline OutputStream& operator<<(OutputStream& output, const char* value) {
  auto length = std::strlen(value);
  output.write(value, length);
  return output;
}

}  // namespace nu
