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

#ifndef NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_

#include "nucleus/streams/input_stream.h"

#include <vector>

namespace nu {

// MemoryInputStream, that does not make a copy of the data passed to it.
class WrappedMemoryInputStream : public InputStream {
public:
  // Construct the stream with the specified data and size.
  WrappedMemoryInputStream(const void* data, usize size);

  // Construct the stream with the specified vector data.
  explicit WrappedMemoryInputStream(const std::vector<char>& data);

  virtual ~WrappedMemoryInputStream() = default;

  // Return a pointer to the source data block from which this stream is
  // reading.
  const void* getData() const { return m_data; }

  // Return the size of the source data (in bytes).
  usize getSize() const { return m_size; }

  // Override: InputStream
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType getLength() override;
  bool isExhausted() override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  // A pointer to the start of the data that we are streaming.
  const void* m_data;

  // The size of the data we are streaming.
  usize m_size;

  // The current position of the stream in the source data.
  usize m_currentPosition{0};

  DISALLOW_IMPLICIT_CONSTRUCTORS(WrappedMemoryInputStream);
};
}  // namespace nu

#endif  // NUCLEUS_STREAMS_WRAPPED_MEMORY_INPUT_STREAM_H_
