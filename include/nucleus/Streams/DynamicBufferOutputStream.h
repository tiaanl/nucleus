#ifndef NUCLEUS_STREAMS_DYNAMIC_BUFFER_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_DYNAMIC_BUFFER_OUTPUT_STREAM_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Macros.h"
#include "nucleus/Streams/OutputStream.h"

namespace nu {

class DynamicBufferOutputStream : public OutputStream {
  NU_DELETE_COPY_AND_MOVE(DynamicBufferOutputStream);

public:
  using BufferType = nu::DynamicArray<U8>;

  DynamicBufferOutputStream();

  const BufferType& buffer() const {
    return m_buffer;
  }

private:
  SizeType write(const void* buffer, SizeType size) override;

private:
  BufferType m_buffer;
  BufferType::SizeType m_currentPosition = 0;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_DYNAMIC_BUFFER_OUTPUT_STREAM_H_
