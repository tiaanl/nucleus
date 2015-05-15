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

#ifndef NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_
#define NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_

#include "nucleus/config.h"
#include "nucleus/files/file_path.h"
#include "nucleus/streams/input_stream.h"

#if OS(WIN)
#include "nucleus/win/windows_mixin.h"
#endif

namespace nu {

class FileInputStream : public InputStream {
public:
#if OS(WIN)
  typedef HANDLE HandleType;
#elif OS(POSIX)
  typedef int HandleType;
#endif

  explicit FileInputStream(const FilePath& fileName);
  ~FileInputStream() override;

  // Returns the file path that this stream is pointing to.
  const FilePath& GetFile() const { return m_path; }

  // Returns the status of the file stream.  The result will be ok if the file
  // opened successfully.  If an error occurred while opening or reading from
  // the file, this will return false.
  bool GetStatus() const;

  // Returns true if the stream couldn't be opened for some reason.
  bool FailedToOpen() const;

  // Returns true if the stream opened without problems.
  bool OpenedOk() const;

  // Override: InputStream
  SizeType getLength() override;
  bool isExhausted() override;
  SizeType getPosition() override;
  bool setPosition(SizeType newPosition) override;
  SizeType read(void* destBuffer, SizeType bytesToRead) override;

private:
  // Open the handle to the file.
  void OpenHandle();

  // Close the handle to the file.
  void CloseHandle();

  // Read data from the file handle.
  SizeType ReadInternal(void* buffer, SizeType numBytes);

  FilePath m_path;
  HandleType m_handle{0};
  SizeType m_currentPosition{0};
  bool m_status{true};
  bool m_needToSeek{true};

  DISALLOW_COPY_AND_ASSIGN(FileInputStream);
};

}  // namespace nu

#endif  // NUCLEUS_STREAMS_FILE_INPUT_STREAM_H_
