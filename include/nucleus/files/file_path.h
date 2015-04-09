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

#ifndef NUCLEUS_FILES_FILE_PATH_H_
#define NUCLEUS_FILES_FILE_PATH_H_

#include <string>

#include "nucleus/config.h"

namespace nu {

class FilePath {
public:
#if OS(POSIX)
  // On most posix platforms, native pathnames are char arrays, and the encoding
  // may or may not be specified.  On Mac OS X, native pathnames are encoded in
  // UTF-8.
  using StringType = std::string;
#else
  // On Windows, native pathnames are wchar_t arrays encoded in UTF-16.
  using StringType = std::u16string;
#endif

  using CharType = StringType::value_type;

  FilePath();
  explicit FilePath(const StringType& path);
  FilePath(const FilePath& other);
  ~FilePath();

  // Make a copy of the path.
  FilePath& operator=(const FilePath& other);

  // Operators.
  bool operator==(const FilePath& other) const;
  bool operator!=(const FilePath& other) const;
  bool operator<(const FilePath& other) const;

  // Return the path as a string.
  const StringType& getPath() const { return m_path; }

  // Return true if the path is empty.
  bool isEmpty() const { return m_path.empty(); }

  // Clear our the path.
  void clear();

private:
  StringType m_path;
};

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_PATH_H_
