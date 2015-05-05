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

#include "nucleus/files/file_utils.h"

#include "nucleus/logging.h"
#include "nucleus/win/windows_mixin.h"

namespace nu {

bool readFileToVector(const std::string& path, std::vector<char>* bufferOut) {
  DCHECK(bufferOut);

  HANDLE fileHandle = ::CreateFileA(
      path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
      OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE)
    return false;

  // We make space for the read buffer on the heap.
  std::vector<char> buf(4096);

  DWORD bytesRead = 0;
  while (::ReadFile(fileHandle, buf.data(), static_cast<DWORD>(buf.size()),
                    &bytesRead, nullptr)) {
    if (bytesRead == 0)
      break;

    bufferOut->reserve(bufferOut->size() + bytesRead);
    std::copy(std::begin(buf), std::begin(buf) + bytesRead,
              std::back_inserter(*bufferOut));
  }

  ::CloseHandle(fileHandle);

  return true;
}

bool writeVectorToFile(const std::string& path, const std::vector<char>& data) {
  HANDLE fileHandle = ::CreateFileA(path.c_str(), GENERIC_WRITE,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                                    CREATE_NEW, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE)
    return false;

  DWORD bytesWritten;
  BOOL result =
      ::WriteFile(fileHandle, data.data(), static_cast<DWORD>(data.size()),
                  &bytesWritten, nullptr);

  ::CloseHandle(fileHandle);

  return bytesWritten == data.size() && result == TRUE;
}

}  // namespace nu
