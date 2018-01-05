
#ifndef NUCLEUS_STREAMS_FILE_OUTPUT_STREAM_H_
#define NUCLEUS_STREAMS_FILE_OUTPUT_STREAM_H_

#include "nucleus/Config.h"
#include "nucleus/FilePath.h"
#include "nucleus/Streams/OutputStream.h"
#include "nucleus/Win/WindowsMixin.h"

namespace nu {

class FileOutputStream : public OutputStream {
public:
#if OS(WIN)
  typedef HANDLE HandleType;
#elif OS(POSIX)
  typedef int HandleType;
#endif

  explicit FileOutputStream(const FilePath& path);
  ~FileOutputStream() override;

  const FilePath& getPath() const {
    return m_path;
  }

  SizeType write(void* buffer, SizeType size) override;

private:
  void openHandle();
  void closeHandle();

  FilePath m_path;
  HandleType m_handle;
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_FILE_OUTPUT_STREAM_H_
