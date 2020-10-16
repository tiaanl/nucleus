
#ifndef NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_

#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Text/DynamicString.h"

namespace nu {

class StringOutputStream : public OutputStream {
public:
  StringOutputStream() : OutputStream{Text}, m_position{0} {}
  ~StringOutputStream() override = default;

  nu::StringView data() const {
    return m_data.view();
  }

  SizeType write(const void* buffer, SizeType size) override;

private:
  DynamicString m_data;
  MemSize m_position;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_
