#pragma once

#include "nucleus/Streams/output_stream.h"
#include "nucleus/config.h"
#include "nucleus/file_path.h"

#if OS(WIN)
#include "nucleus/Win/includes.h"
#endif

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

  SizeType write(const void* buffer, SizeType size) override;

private:
  void openHandle();
  void closeHandle();

  FilePath m_path;
  HandleType m_handle;
};

}  // namespace nu
