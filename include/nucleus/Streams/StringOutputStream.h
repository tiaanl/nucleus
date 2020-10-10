
#ifndef NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_

#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Text/DynamicString.h"

namespace nu {

class StringOutputStream : public OutputStream {
public:
  StringOutputStream() : OutputStream{Text} {}
  ~StringOutputStream() override = default;

  const nu::DynamicString& getString() const {
    return m_text;
  }

  SizeType write(const void* buffer, SizeType size) override;

private:
  DynamicString m_text;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_STRING_OUTPUT_STREAM_H_
