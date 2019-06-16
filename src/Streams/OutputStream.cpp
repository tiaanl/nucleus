
#include "nucleus/Streams/OutputStream.h"

#include "nucleus/MemoryDebug.h"

#include <cstdio>
#include <cstring>

namespace nu {

namespace {

MemSize writeSignedNumberToBuffer(I8* buffer, MemSize bufferSize, I64 value) {
#if COMPILER(MSVC)
  return sprintf_s(buffer, bufferSize, "%lld", value);
#else
  return sprintf(buffer, "%lld", value);
#endif
}

MemSize writeUnsignedNumberToBuffer(I8* buffer, MemSize bufferSize, U64 value) {
#if COMPILER(MSVC)
  return sprintf_s(buffer, bufferSize, "%llu", value);
#else
  return sprintf(buffer, "%llu", value);
#endif
}

MemSize writeFloatToBuffer(I8* buffer, MemSize bufferSize, F64 value) {
#if COMPILER(MSVC)
  return sprintf_s(buffer, bufferSize, "%.3f", value);
#else
  return sprintf(buffer, "%.3f", value);
#endif
}

}  // namespace

void OutputStream::writeBool(bool data) {
  if (m_mode == Text) {
    writeString(data ? "true" : "false");
  } else {
    writeU8(data ? 1 : 0);
  }
}

void OutputStream::writeU8(U8 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU16(U16 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU32(U32 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU64(U64 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeI8(I8 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeI16(I16 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeI32(I32 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeI64(I64 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeF32(F32 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeFloatToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeF64(F64 data) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeFloatToBuffer(buffer, sizeof(buffer), data);
    write(buffer, bytesWritten);
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeString(const StringView& data) {
  write(data.getData(), data.getLength());
}

}  // namespace nu
