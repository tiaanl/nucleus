
#include "nucleus/Streams/FileInputStream.h"

#include "nucleus/Logging.h"

#include <algorithm>

#if OS(POSIX)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace nu {

namespace detail {

I64 setFileInputStreamPosition(FileInputStream::HandleType handle, I64 pos) {
#if OS(WIN)
  LARGE_INTEGER li = {};
  li.QuadPart = pos;
  li.LowPart = ::SetFilePointer(static_cast<HANDLE>(handle), static_cast<LONG>(li.LowPart),
                                &li.HighPart, FILE_BEGIN);
  return li.QuadPart;
#elif OS(POSIX)
  if (handle != 0 && lseek(handle, pos, SEEK_SET) == pos) {
    return pos;
  }
  return -1;
#endif
}

}  // namespace detail

#if OS(WIN)
// static
FileInputStream::HandleType FileInputStream::kInvalidHandle = INVALID_HANDLE_VALUE;
#elif OS(POSIX)
// static
HandleType FileInputStream::kInvalidHandle = 0;
#endif

FileInputStream::FileInputStream(const FilePath& path) : m_path(path) {
  openHandle();
}

FileInputStream::~FileInputStream() {
  closeHandle();
}

bool FileInputStream::openedOk() const {
  return m_status;
}

FileInputStream::SizeType FileInputStream::getSize() {
#if OS(WIN)
  SizeType fileSize = 0;
  DWORD lowSize = ::GetFileSize(static_cast<HANDLE>(m_handle), nullptr);
  fileSize = lowSize;
  return fileSize;
#elif OS(POSIX)
  long size = 0;
  FILE* fp = fopen(m_path.getPath().getData(), "rb");
  if (fp != 0) {
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);
  }

  return static_cast<SizeType>(size);
#endif
}

FileInputStream::SizeType FileInputStream::read(void* destBuffer, SizeType maxBytesToRead) {
  if (!openedOk()) {
    LOG(Fatal) << "Trying to read from an invalid stream.";
    return 0;
  }
  DCHECK(destBuffer != nullptr);

  if (m_needToSeek) {
    if (detail::setFileInputStreamPosition(m_handle, m_currentPosition) < 0)
      return 0;

    m_needToSeek = false;
  }

  MemSize num = readInternal(destBuffer, static_cast<MemSize>(maxBytesToRead));
  m_currentPosition += num;

  return static_cast<SizeType>(num);
}

bool FileInputStream::isExhausted() {
  return m_currentPosition >= getSize();
}

FileInputStream::SizeType FileInputStream::getPosition() {
  return m_currentPosition;
}

bool FileInputStream::setPosition(SizeType newPosition) {
  DCHECK(openedOk());

  if (newPosition != m_currentPosition) {
    newPosition = std::max(newPosition, static_cast<SizeType>(0));
    newPosition = std::min(newPosition, getSize());

    m_needToSeek |= (m_currentPosition != newPosition);
    m_currentPosition = newPosition;
  }

  return true;
}

void FileInputStream::openHandle() {
#if OS(WIN)
  HANDLE h = ::CreateFileA(m_path.getPath().getData(), GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
  if (h != INVALID_HANDLE_VALUE) {
    m_handle = (void*)h;
  } else {
    m_status = false;
  }
#elif OS(POSIX)
  int f = open(m_path.getPath().getData(), O_RDONLY, 00644);
  if (f != -1) {
    m_handle = f;
  } else {
    m_status = false;
  }
#endif
}

void FileInputStream::closeHandle() {
#if OS(WIN)
  ::CloseHandle(static_cast<HANDLE>(m_handle));
#elif OS(POSIX)
  if (m_handle != 0) {
    close(m_handle);
    m_handle = 0;
  }
#endif
}

FileInputStream::SizeType FileInputStream::readInternal(void* buffer, SizeType numBytes) {
#if OS(WIN)
  if (m_handle != nullptr) {
    DWORD actualNum = 0;
    if (::ReadFile(static_cast<HANDLE>(m_handle), buffer, static_cast<DWORD>(numBytes), &actualNum,
                   nullptr) == FALSE) {
      m_status = false;
    }

    return static_cast<MemSize>(actualNum);
  }

  return 0;
#elif OS(POSIX)
  ssize_t result = 0;

  if (m_handle != 0) {
    result = ::read(m_handle, buffer, numBytes);

    if (result < 0) {
      m_status = false;
      result = 0;
    }
  }

  return (size_t)result;
#endif
}

}  // namespace nu
