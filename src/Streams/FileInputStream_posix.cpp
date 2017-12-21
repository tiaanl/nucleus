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

#ifndef NUCLEUS_STREAMS_FILE_INPUT_STREAM_POSIX_H_
#define NUCLEUS_STREAMS_FILE_INPUT_STREAM_POSIX_H_

#include "nucleus/Streams/FileInputStream.h"

#if OS(POSIX)
#include <fcntl.h>
#include <unistd.h>
#endif  // OS(POSIX)

namespace nu {

namespace detail {

I64 setFileInputStreamPosition(FileInputStream::HandleType handle, I64 pos) {
  if (handle != 0 && lseek(handle, pos, SEEK_SET) == pos) {
    return pos;
  }
  return -1;
}

}  // namespace detail

FileInputStream::SizeType FileInputStream::getLength() {
  long size = 0;
  FILE* fp = fopen(m_path.getPath().c_str(), "rb");
  if (fp != 0) {
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);
  }

  return static_cast<SizeType>(size);
}

void FileInputStream::openHandle() {
  int f = open(m_path.getPath().c_str(), O_RDONLY, 00644);
  if (f != -1) {
    m_handle = f;
  } else {
    m_status = false;
  }
}

void FileInputStream::closeHandle() {
  if (m_handle != 0) {
    close(m_handle);
    m_handle = 0;
  }
}

USize FileInputStream::readInternal(void* buffer, USize numBytes) {
  ssize_t result = 0;

  if (m_handle != 0) {
    result = ::read(m_handle, buffer, numBytes);

    if (result < 0) {
      m_status = false;
      result = 0;
    }
  }

  return (size_t)result;
}

}  // namespace nu

#endif  // NUCLEUS_STREAMS_FILE_INPUT_STREAM_POSIX_H_