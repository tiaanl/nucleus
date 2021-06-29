#pragma once

#include "nucleus/containers/dynamic_array.h"
#include "nucleus/streams/output_stream.h"
#include "nucleus/macros.h"

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

  BufferType m_buffer;
  BufferType::SizeType m_currentPosition = 0;
};

}  // namespace nu
