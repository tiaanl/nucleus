
#ifndef NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_

#include "nucleus/Containers/ArrayView.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

class ArrayInputStream : public InputStream {
  NU_DELETE_COPY_AND_MOVE(ArrayInputStream);

public:
  ArrayInputStream(ArrayView<U8> buffer);
  virtual ~ArrayInputStream() = default;

  // Override: InputStream
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType getSize() override;
  bool isExhausted() override;
  SizeType read(void* destination, SizeType bytesToRead) override;

private:
  // Where the buffer data is stored.
  ArrayView<U8> m_buffer;

  // The current position inside the data buffer.
  MemSize m_position = 0;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_
