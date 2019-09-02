
#ifndef NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

class MemoryInputStream : public InputStream {
public:
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

  DELETE_COPY_AND_MOVE(MemoryInputStream);
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_
