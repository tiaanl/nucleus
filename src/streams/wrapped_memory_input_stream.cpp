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

#include "nucleus/streams/wrapped_memory_input_stream.h"

#include <algorithm>
#include <cstring>

#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

WrappedMemoryInputStream::WrappedMemoryInputStream(const void* data, usize size)
  : m_data(data), m_size(size) {
}

WrappedMemoryInputStream::WrappedMemoryInputStream(
    const std::vector<char>& data)
  : m_data(data.data()), m_size(data.size()) {
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::getLength() {
  return m_size;
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::read(
    void* buffer, SizeType bytesToRead) {
  DCHECK(bytesToRead >= 0);

  SizeType num = std::min(bytesToRead, getBytesRemaining());

  std::memcpy(buffer, static_cast<const u8*>(m_data) + m_currentPosition,
              static_cast<usize>(num));
  m_currentPosition += num;

  return num;
}

bool WrappedMemoryInputStream::isExhausted() {
  return m_currentPosition >= m_size;
}

bool WrappedMemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = std::max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = std::min(newPosition, m_size);
  return true;
}

WrappedMemoryInputStream::SizeType WrappedMemoryInputStream::getPosition() {
  return m_currentPosition;
}

}  // namespace nu
