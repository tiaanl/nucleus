
#include "nucleus/Streams/FileOutputStream.h"

#include <nucleus/Text/Utils.h>

#if OS(POSIX)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace nu {

FileOutputStream::FileOutputStream(const FilePath& path)
  : OutputStream{OutputStream::Binary}, m_path{path} {
  openHandle();
}

FileOutputStream::~FileOutputStream() {
  closeHandle();
}

void FileOutputStream::openHandle() {
  auto path = nu::zeroTerminated(m_path.getPath());

#if OS(WIN)
  HANDLE h = ::CreateFileA(path.data(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
  if (h != INVALID_HANDLE_VALUE) {
    m_handle = (void*)h;
  }
#elif OS(POSIX)
  int f = open(path.data(), O_WRONLY, 00644);
  if (f != -1) {
    m_handle = f;
  } else {
    // m_status = false;
  }
#endif  // OS(POSIX)
}

void FileOutputStream::closeHandle() {
#if OS(WIN)
  ::CloseHandle(m_handle);
#elif OS(POSIX)
  if (m_handle != 0) {
    close(m_handle);
    m_handle = 0;
  }
#endif  // OS(*)
}

#if OS(WIN)
FileOutputStream::SizeType FileOutputStream::write(const void* buffer, SizeType size) {
  // static_assert(sizeof(DWORD) == sizeof(SizeType), "Sizes must match");

  DWORD bytesWritten = 0;
  if (m_handle) {
    if (!::WriteFile(m_handle, buffer, static_cast<DWORD>(size), &bytesWritten, nullptr)) {
      LOG(Error) << "Could not write to file.";
    }
  }

  return static_cast<SizeType>(bytesWritten);
}
#elif OS(POSIX)
FileOutputStream::SizeType FileOutputStream::write(const void*, SizeType) {
  //#error "Not implemented!"
  return 0;
}
#endif

}  // namespace nu
