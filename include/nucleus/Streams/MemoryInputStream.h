
#ifndef NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

class MemoryInputStream : public InputStream {
public:
  MemoryInputStream(const void* src, USize srcDataSize);
  explicit MemoryInputStream(const nu::DynamicArray<I8>& data);

  virtual ~MemoryInputStream();

  // Return a pointer to the source data block from which this stream is
  // reading.
  const void* getData() const {
    return m_buffer.getData();
  }

  // Returns the number of bytes of source data in the block from which this
  // stream is reading.
  USize GetDataSize() const {
    return m_buffer.getSize();
  }

  // Override: InputStream
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType getLength() override;
  bool isExhausted() override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  void createInternalCopy(const I8* data, USize dataSize);

  nu::DynamicArray<I8> m_buffer;
  USize m_currentPosition;

  DISALLOW_COPY_AND_ASSIGN(MemoryInputStream);
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_