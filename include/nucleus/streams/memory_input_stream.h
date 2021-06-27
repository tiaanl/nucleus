#pragma once

#include "nucleus/Containers/dynamic_array.h"
#include "nucleus/Streams/input_stream.h"

namespace nu {

class MemoryInputStream : public InputStream {
public:
  NU_DELETE_COPY_AND_MOVE(MemoryInputStream);

  MemoryInputStream(void* src, MemSize srcDataSize);
  explicit MemoryInputStream(nu::DynamicArray<U8> data);

  ~MemoryInputStream() override;

  // Return a pointer to the source data block from which this stream is reading.
  const void* getData() const {
    return m_buffer.data();
  }

  // Returns the number of bytes of source data in the block from which this stream is reading.
  MemSize GetDataSize() const {
    return m_buffer.size();
  }

  // Override: InputStream
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType getSize() override;
  bool isExhausted() override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  void createInternalCopy(const I8* data, MemSize dataSize);

  nu::DynamicArray<U8> m_buffer;
  MemSize m_currentPosition;
};

}  // namespace nu
