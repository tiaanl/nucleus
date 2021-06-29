
#include "nucleus/streams/output_stream.h"

#include <cstdio>

namespace nu {

namespace {

#if COMPILER(MSVC)
MemSize writeSignedNumberToBuffer(I8* buffer, MemSize bufferSize, I64 value) {
  return sprintf_s(buffer, bufferSize, "%lld", value);
#else
MemSize writeSignedNumberToBuffer(I8* buffer, MemSize, I64 value) {
  return sprintf(buffer, "%lld", value);
#endif
}

#if COMPILER(MSVC)
MemSize writeUnsignedNumberToBuffer(I8* buffer, MemSize bufferSize, U64 value) {
  return sprintf_s(buffer, bufferSize, "%llu", value);
#else
MemSize writeUnsignedNumberToBuffer(I8* buffer, MemSize, U64 value) {
  return sprintf(buffer, "%llu", value);
#endif
}

#if COMPILER(MSVC)
MemSize writeFloatToBuffer(I8* buffer, MemSize bufferSize, F64 value) {
  return sprintf_s(buffer, bufferSize, "%.3f", value);
#else
MemSize writeFloatToBuffer(I8* buffer, MemSize, F64 value) {
  return sprintf(buffer, "%.3f", value);
#endif
}

}  // namespace

void OutputStream::writeBool(bool value) {
  if (m_mode == Text) {
    if (value) {
      write((void*)"true", 4);
    } else {
      write((void*)"false", 5);
    }
  } else {
    writeU8(value ? 1 : 0);
  }
}

void OutputStream::writeU8(U8 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeU16(U16 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeU32(U32 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeU64(U64 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeUnsignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeI8(I8 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeI16(I16 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeI32(I32 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeI64(I64 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeSignedNumberToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeF32(F32 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeFloatToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

void OutputStream::writeF64(F64 value) {
  if (m_mode == Text) {
    I8 buffer[32];
    auto bytesWritten = writeFloatToBuffer(buffer, sizeof(buffer), value);
    write(buffer, bytesWritten);
  } else {
    write(&value, sizeof(value));
  }
}

}  // namespace nu
