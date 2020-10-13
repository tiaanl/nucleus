#ifndef NUCLEUS_STREAMS_WRAPPED_DYNAMIC_ARRAY_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_WRAPPED_DYNAMIC_ARRAY_OUTPUT_STREAM_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Macros.h"
#include "nucleus/Streams/OutputStream.h"

namespace nu {

class WrappedDynamicArrayOutputStream : public OutputStream {
public:
  NU_DELETE_COPY_AND_MOVE(WrappedDynamicArrayOutputStream);

  explicit WrappedDynamicArrayOutputStream(DynamicArray<U8>& dynamicArray)
    : OutputStream{OutputStreamMode::Binary}, m_dynamicArray{dynamicArray} {}

private:
  SizeType write(const void* buffer, SizeType size) override {
    const U8* start = static_cast<const U8*>(buffer);
    const U8* end = start + size;
    m_dynamicArray.pushBack(start, end);
    return 0;
  }

private:
  DynamicArray<U8>& m_dynamicArray;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_WRAPPED_DYNAMIC_ARRAY_OUTPUT_STREAM_H_
