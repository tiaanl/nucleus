
#include "nucleus/streams/FileInputStream.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace detail {

I64 setFileInputStreamPosition(FileInputStream::HandleType handle, I64 pos) {
  LARGE_INTEGER li = {0};
  li.QuadPart = pos;
  li.LowPart = ::SetFilePointer(static_cast<HANDLE>(handle), static_cast<LONG>(li.LowPart), &li.HighPart, FILE_BEGIN);
  return li.QuadPart;
}

}  // namespace detail

FileInputStream::SizeType FileInputStream::getLength() {
  SizeType fileSize = 0;
  DWORD lowSize = ::GetFileSize(static_cast<HANDLE>(m_handle), nullptr);
  fileSize = lowSize;
  return fileSize;
}

void FileInputStream::openHandle() {
  HANDLE h = ::CreateFileW((LPCWSTR)m_path.getPath().getRawBytes(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                           nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
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
  if (m_handle != nullptr) {
    DWORD actualNum = 0;
    if (::ReadFile(static_cast<HANDLE>(m_handle), buffer, static_cast<DWORD>(numBytes), &actualNum, nullptr) == FALSE) {
      m_status = false;
    }

    return static_cast<USize>(actualNum);
  }

  return 0;
}

}  // namespace nu
