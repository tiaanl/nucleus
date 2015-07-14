// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_

#include <vector>

#include "nucleus/streams/input_stream.h"

namespace nu {

class MemoryInputStream : public InputStream {
public:
  MemoryInputStream(const void* src, usize srcDataSize);
  explicit MemoryInputStream(const std::vector<char>& data);

  virtual ~MemoryInputStream();

  // Return a pointer to the source data block from which this stream is
  // reading.
  const void* getData() const { return &m_buffer[0]; }

  // Returns the number of bytes of source data in the block from which this
  // stream is reading.
  usize GetDataSize() const { return m_buffer.size(); }

  // Override: InputStream
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType getLength() override;
  bool isExhausted() override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  void createInternalCopy(const char* data, usize dataSize);

  std::vector<char> m_buffer;
  usize m_currentPosition;

  DISALLOW_COPY_AND_ASSIGN(MemoryInputStream);
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_MEMORY_INPUT_STREAM_H_
