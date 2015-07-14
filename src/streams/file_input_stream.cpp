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

#include "nucleus/streams/file_input_stream.h"

#include <algorithm>

#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

namespace detail {

i64 setFileInputStreamPosition(FileInputStream::HandleType handle, i64 pos);

}  // namespace detail

FileInputStream::FileInputStream(const FilePath& path) : m_path(path) {
  openHandle();
}

FileInputStream::~FileInputStream() {
  closeHandle();
}

bool FileInputStream::openedOk() const {
  return m_status;
}

FileInputStream::SizeType FileInputStream::read(void* destBuffer,
                                                SizeType maxBytesToRead) {
  if (!openedOk()) {
    LOG(Warning) << "Trying to read from an invalid stream.";
    NOTREACHED() << "Do not read from an invalid stream.";
    return 0;
  }
  DCHECK(destBuffer != NULL && maxBytesToRead >= 0);

  if (m_needToSeek) {
    if (detail::setFileInputStreamPosition(m_handle, m_currentPosition) < 0)
      return 0;

    m_needToSeek = false;
  }

  usize num = readInternal(destBuffer, static_cast<usize>(maxBytesToRead));
  m_currentPosition += num;

  return static_cast<SizeType>(num);
}

bool FileInputStream::isExhausted() {
  return m_currentPosition >= getLength();
}

FileInputStream::SizeType FileInputStream::getPosition() {
  return m_currentPosition;
}

bool FileInputStream::setPosition(SizeType newPosition) {
  DCHECK(openedOk());

  if (newPosition != m_currentPosition) {
    newPosition = std::max(newPosition, static_cast<SizeType>(0));
    newPosition = std::min(newPosition, getLength());

    m_needToSeek |= (m_currentPosition != newPosition);
    m_currentPosition = newPosition;
  }

  return true;
}
}  // namespace nu
