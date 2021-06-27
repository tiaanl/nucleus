#pragma once

#include "nucleus/Streams/output_stream.h"

namespace nu {

class ConsoleOutputStream : public OutputStream {
public:
  ConsoleOutputStream() : OutputStream(OutputStream::Text) {}
  ~ConsoleOutputStream() override = default;

  SizeType write(const void *buffer, SizeType size) override;
};

}  // namespace nu
