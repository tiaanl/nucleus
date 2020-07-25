
#ifndef NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_

#include "nucleus/Config.h"
#include "nucleus/FilePath.h"
#include "nucleus/Streams/InputStream.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#endif

namespace nu {

class FileInputStream : public InputStream {
public:
  NU_DELETE_COPY_AND_MOVE(FileInputStream);

#if OS(WIN)
  typedef HANDLE HandleType;
#elif OS(POSIX)
  typedef int HandleType;
#endif

  explicit FileInputStream(const FilePath& fileName);
  ~FileInputStream() override;

  // Returns the file path that this stream is pointing to.
  const FilePath& getFile() const {
    return m_path;
  }

  // Returns the status of the file stream.  The result will be ok if the file
  // opened successfully.  If an error occurred while opening or reading from
  // the file, this will return false.
  bool getStatus() const;

  // Returns true if the stream couldn't be opened for some reason.
  bool failedToOpen() const;

  // Returns true if the stream opened without problems.
  bool openedOk() const;

  // Override: InputStream
  SizeType getSize() override;
  bool isExhausted() override;
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  // Represents an invalid state for the internal handle.
  static HandleType kInvalidHandle;

  // Open the handle to the file.
  void openHandle();

  // Close the handle to the file.
  void closeHandle();

  // Read data from the file handle.
  SizeType readInternal(void* buffer, SizeType numBytes);

  FilePath m_path;
  HandleType m_handle = kInvalidHandle;
  SizeType m_currentPosition = 0;
  bool m_status = true;
  bool m_needToSeek = true;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_
