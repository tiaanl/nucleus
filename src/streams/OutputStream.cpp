
#include "nucleus/streams/OutputStream.h"

namespace nu {

void OutputStream::writeBool(bool data) {
  writeU8(data ? 1 : 0);
}

void OutputStream::writeU8(U8 data) {
  write(&data, sizeof(data));
}

void OutputStream::writeInt16(I16 data) {
  write(&data, sizeof(data));
}

void OutputStream::writeInt32(I32 data) {
  write(&data, sizeof(data));
}

void OutputStream::writeInt64(I64 data) {
  write(&data, sizeof(data));
}

void OutputStream::writeFloat(F32 data) {
  write(&data, sizeof(data));
}

void OutputStream::writeDouble(F64 data) {
  write(&data, sizeof(data));
}

}  // namespace nu
