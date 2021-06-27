#pragma once

#include "nucleus/Streams/output_stream.h"
#include "nucleus/Text/dynamic_string.h"

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
