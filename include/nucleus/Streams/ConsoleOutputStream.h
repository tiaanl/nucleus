
#ifndef NUCLEUS_STREAMS_CONSOLE_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_CONSOLE_OUTPUT_STREAM_H_

#include "nucleus/Streams/OutputStream.h"

namespace nu {

class ConsoleOutputStream : public OutputStream {
public:
  ConsoleOutputStream() : OutputStream(OutputStream::Text) {}
  ~ConsoleOutputStream() override = default;

  SizeType write(void *buffer, SizeType size) override;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_CONSOLE_OUTPUT_STREAM_H_
