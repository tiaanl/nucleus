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

#include "nucleus/streams/memory_input_stream.h"

#include <algorithm>
#include <cstring>

#include "nucleus/logging.h"
#include "nucleus/types.h"
#include "nucleus/utils/stl.h"

namespace nu {

MemoryInputStream::MemoryInputStream(const void* sourceData,
                                     USize sourceDataSize)
  : m_currentPosition(0) {
  createInternalCopy(static_cast<const char*>(sourceData), sourceDataSize);
}

MemoryInputStream::MemoryInputStream(const std::vector<char>& data)
  : m_currentPosition(0) {
  createInternalCopy(&data[0], data.size());
}

MemoryInputStream::~MemoryInputStream() {
}

MemoryInputStream::SizeType MemoryInputStream::getLength() {
  return m_buffer.size();
}

MemoryInputStream::SizeType MemoryInputStream::read(void* buffer,
                                                    SizeType bytesToRead) {
  DCHECK(bytesToRead >= 0);

  SizeType num = std::min(bytesToRead, m_buffer.size() - m_currentPosition);
  if (num <= 0)
    return 0;

  memcpy(buffer, static_cast<const char*>(&m_buffer[0]) + m_currentPosition,
         static_cast<USize>(num));
  m_currentPosition += static_cast<USize>(num);

  return num;
}

bool MemoryInputStream::isExhausted() {
  return m_currentPosition >= m_buffer.size();
}

bool MemoryInputStream::setPosition(SizeType newPosition) {
  m_currentPosition = std::max(newPosition, static_cast<SizeType>(0));
  m_currentPosition = std::min(newPosition, m_buffer.size());
  return true;
}

MemoryInputStream::SizeType MemoryInputStream::getPosition() {
  return m_currentPosition;
}

void MemoryInputStream::createInternalCopy(const char* data, USize dataSize) {
  memcpy(vectorAsArray(&m_buffer, dataSize), data, dataSize);
}
}  // namespace nu
