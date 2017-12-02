
#include "nucleus/streams/FileOutputStream.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

FileOutputStream::FileOutputStream(const FilePath& path) : m_path(path) {
  openHandle();
}

FileOutputStream::~FileOutputStream() {
  closeHandle();
}

void FileOutputStream::openHandle() {
  HANDLE h = ::CreateFileW((LPCWSTR)m_path.getPath().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
  if (h != INVALID_HANDLE_VALUE) {
    m_handle = (void*)h;
  }
}

void FileOutputStream::closeHandle() {
  ::CloseHandle(m_handle);
}

FileOutputStream::SizeType FileOutputStream::write(void* buffer, SizeType size) {
  DWORD bytesWritten = 0;
  if (m_handle) {
    ::WriteFile(m_handle, buffer, size, &bytesWritten, nullptr);
  }

  return static_cast<SizeType>(bytesWritten);
}

}  // namespace nu
