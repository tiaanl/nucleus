
#include "nucleus/Streams/OutputStream.h"

#include "nucleus/MemoryDebug.h"

#include <cstdio>
#include <cstring>

namespace nu {

namespace {

MemSize writeNumberToBuffer(I8* buffer, MemSize, I64 value) {
  return sprintf(buffer, "%lld", value);
}

MemSize writeFloatToBuffer(I8* buffer, MemSize, F64 value) {
  return sprintf(buffer, "%.3f", value);
}

}  // namespace

void OutputStream::writeBool(bool data) {
  writeU8(data ? 1 : 0);
}

void OutputStream::writeU8(U8 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU16(U16 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU32(U32 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeU64(U64 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  }
  else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeInt16(I16 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeInt32(I32 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeInt64(I64 data) {
  if (m_mode == Text) {
    writeNumberToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeFloat32(F32 data) {
  if (m_mode == Text) {
    writeFloatToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeFloat64(F64 data) {
  if (m_mode == Text) {
    writeFloatToBuffer(m_buffer, sizeof(m_buffer), data);
    write(m_buffer, std::strlen(m_buffer));
  } else {
    write(&data, sizeof(data));
  }
}

void OutputStream::writeCString(const char* data) {
  write(const_cast<char*>(data), strlen(data));
}

}  // namespace nu
