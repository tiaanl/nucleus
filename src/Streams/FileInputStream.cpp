
#include "nucleus/Streams/FileInputStream.h"

#include <algorithm>

#include "nucleus/Logging.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace detail {

I64 setFileInputStreamPosition(FileInputStream::HandleType handle, I64 pos);

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

}  // namespace nu
