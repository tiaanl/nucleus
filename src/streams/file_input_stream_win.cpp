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

#include "nucleus/win/windows_mixin.h"

namespace nu {

namespace detail {

int64_t setFileInputStreamPosition(FileInputStream::HandleType handle,
                                   int64_t pos) {
  LARGE_INTEGER li;
  li.QuadPart = pos;
  li.LowPart =
      ::SetFilePointer(static_cast<HANDLE>(handle),
                       static_cast<LONG>(li.LowPart), &li.HighPart, FILE_BEGIN);
  return li.QuadPart;
}

}  // namespace detail

FileInputStream::SizeType FileInputStream::getLength() {
  int64_t fileSize = 0;
  DWORD lowSize = ::GetFileSize(static_cast<HANDLE>(m_handle), NULL);
  fileSize = lowSize;
  return fileSize;
}

void FileInputStream::openHandle() {
  HANDLE h =
      ::CreateFileW((LPCWSTR)m_path.getPath().c_str(), GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
  if (h != INVALID_HANDLE_VALUE) {
    m_handle = (void*)h;
  } else {
    m_status = false;
  }
}

void FileInputStream::closeHandle() {
  ::CloseHandle(static_cast<HANDLE>(m_handle));
}

FileInputStream::SizeType FileInputStream::readInternal(void* buffer,
                                                        SizeType numBytes) {
  if (m_handle != NULL) {
    DWORD actualNum = 0;
    if (!::ReadFile(static_cast<HANDLE>(m_handle), buffer,
                    static_cast<DWORD>(numBytes), &actualNum, 0)) {
      m_status = false;
    }

    return static_cast<size_t>(actualNum);
  }

  return 0;
}

}  // namespace nu
