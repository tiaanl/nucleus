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

#include "nucleus/files/file_path.h"

#include "nucleus/strings/string_utils.h"

namespace nu {

namespace {

using StringType = FilePath::StringType;

// If this FilePath contains a drive letter specification, returns the position
// of the last character of the drive letter specification, otherwise returns
// npos.  This can only be true on Windows, when a pathname begins with a letter
// followed by a colon.  On other platforms, this always returns npos.
StringType::size_type FindDriveLetter(const StringType& path) {
#if OS(WIN)
  if (path.length() >= 2 && path[1] == L':' &&
      ((path[0] >= L'A' && path[0] <= L'Z') ||
       (path[0] >= L'a' && path[0] <= L'z'))) {
    return 1;
  }
#endif  // FILE_PATH_USES_DRIVE_LETTERS
  return StringType::npos;
}

#if OS(WIN)
bool EqualDriveLetterCaseInsensitive(const StringType& left,
                                     const StringType& right) {
  auto leftLetterPos = FindDriveLetter(left);
  auto rightLetterPos = FindDriveLetter(right);

  if (leftLetterPos == StringType::npos || rightLetterPos == StringType::npos) {
    return leftLetterPos == rightLetterPos;
  }

  StringType leftLetter{left.substr(0, leftLetterPos + 1)};
  StringType rightLetter{right.substr(0, rightLetterPos + 1)};
#if 0
  if (!StartsWith(leftLetter, rightLetter, false))
    return false;
#endif  // 0

  StringType leftRest{left.substr(leftLetterPos + 1)};
  StringType rightRest{right.substr(rightLetterPos + 1)};

  return leftRest == rightRest;
}
#endif  // OS(WIN)

}  // namespace

FilePath::FilePath() {
}

FilePath::FilePath(const FilePath& other) : m_path(other.m_path) {
}

FilePath::FilePath(const StringType& path) : m_path(path) {
}

FilePath::~FilePath() {
}

FilePath& FilePath::operator=(const FilePath& other) {
  m_path = other.m_path;
  return *this;
}

bool FilePath::operator==(const FilePath& other) const {
#if OS(WIN)
  return EqualDriveLetterCaseInsensitive(m_path, other.m_path);
#else
  return m_path == other.m_path;
#endif
}

bool FilePath::operator!=(const FilePath& other) const {
  return !(*this == other);
}

}  // namespace nu
