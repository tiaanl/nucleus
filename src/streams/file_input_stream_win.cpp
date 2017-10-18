
#include "nucleus/streams/file_input_stream.h"

#include "nucleus/win/windows_mixin.h"

namespace nu {

namespace detail {

I64 setFileInputStreamPosition(FileInputStream::HandleType handle, I64 pos) {
  LARGE_INTEGER li;
  li.QuadPart = pos;
  li.LowPart = ::SetFilePointer(static_cast<HANDLE>(handle), static_cast<LONG>(li.LowPart), &li.HighPart, FILE_BEGIN);
  return li.QuadPart;
}

}  // namespace detail

FileInputStream::SizeType FileInputStream::getLength() {
  I64 fileSize = 0;
  DWORD lowSize = ::GetFileSize(static_cast<HANDLE>(m_handle), NULL);
  fileSize = lowSize;
  return fileSize;
}

void FileInputStream::openHandle() {
  HANDLE h = ::CreateFileW((LPCWSTR)m_path.getPath().c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
  if (h != INVALID_HANDLE_VALUE) {
    m_handle = (void*)h;
  } else {
    m_status = false;
  }
}

void FileInputStream::closeHandle() {
  ::CloseHandle(static_cast<HANDLE>(m_handle));
}

FileInputStream::SizeType FileInputStream::readInternal(void* buffer, SizeType numBytes) {
  if (m_handle != NULL) {
    DWORD actualNum = 0;
    if (!::ReadFile(static_cast<HANDLE>(m_handle), buffer, static_cast<DWORD>(numBytes), &actualNum, 0)) {
      m_status = false;
    }

    return static_cast<USize>(actualNum);
  }

  return 0;
}

}  // namespace nu
