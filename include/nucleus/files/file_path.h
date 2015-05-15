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
  using StringType = std::wstring;
#endif

  using CharType = StringType::value_type;

  // Null-terminated array of separators used to separate components in
  // hierarchical paths.  Each character in this array is a valid separator, but
  // kSeparators[0] is treated as the canonical separator and will be used when
  // composing pathnames.
  static const CharType kSeparators[];

  // A special path component meaning "this directory."
  static const CharType kCurrentDirectory[];

  // A special path component meaning "the parent directory."
  static const CharType kParentDirectory[];

  // The character used to identify a file extension.
  static const CharType kExtensionSeparator;

  // Returns true if ch is a FilePath separator.
  static bool isSeparator(CharType ch);

  FilePath();
  FilePath(const StringType& path);
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

  // Returns a FilePath corresponding to the directory containing ht epath named
  // by this object, stripping away the file component.
  FilePath dirName() const;

  // Returns a FilePath corresponding to the last path component of this object,
  // either a file or a directory.
  FilePath baseName() const;

  // Returns a FilePath by appending a separator and the supplied path component
  // to this object's path.  Append takes care to avoid adding excessive
  // separators if this object's path already ends with a separator.
  FilePath append(const StringType& component) const;
  FilePath append(const FilePath& component) const;

private:
  // Remove trailing separators from this object.
  void stripTrailingSeparatorsInternal();

  // A string representation of the path in this object.
  StringType m_path;
};

// Macro for string literal initialization of FilePath::CharType[].
#if OS(POSIX)
#define FILE_PATH_LITERAL(Str) Str
#elif OS(WIN)
#define FILE_PATH_LITERAL(Str) L##Str
#endif

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_PATH_H_
