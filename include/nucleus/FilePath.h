
#ifndef NUCLEUS_FILES_FILE_PATH_H_
#define NUCLEUS_FILES_FILE_PATH_H_

#include <ostream>

#include "nucleus/Config.h"
#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Text/DynamicString.h"

#if OS(WIN)
#define FILE_PATH_USES_DRIVE_LETTERS 1
#endif  // OS(WIN)

#if OS(WIN)
#define FILE_PATH_USES_WIN_SEPARATORS 1
#endif  // OS(WIN)

namespace nu {

class FilePath {
public:
  // Returns true if ch is a FilePath separator.
  static bool isSeparator(Char ch);

  // Normalize separators to all be the default separator for the platform.
  static FilePath normalizeSeparators(StringView path);

  FilePath();
  FilePath(StringView path);
  FilePath(const FilePath& other);
  FilePath(FilePath&&) = default;

  FilePath& operator=(const FilePath& other);
  FilePath& operator=(FilePath&&) = default;

  bool operator==(const FilePath& other) const;
  bool operator!=(const FilePath& other) const;

  // Return the path as a string.
  StringView getPath() const {
    return m_path.view();
  }

  // Clear the path.
  void clear();

  // Returns a FilePath corresponding to the directory containing the path named by this object,
  // stripping away the file component.
  FilePath dirName() const;

  // Returns a FilePath corresponding to the last path component of this object, either a file or a
  // directory.
  FilePath baseName() const;

  // Returns a FilePath by appending a separator (if needed) and the supplied path component.
  FilePath append(StringView component) const;
  FilePath append(const FilePath& component) const;

private:
  // Remove trailing separators from this object.
  void stripTrailingSeparators();

  // A string representation of the path in this object.
  DynamicString m_path;
};

std::ostream& operator<<(std::ostream& os, const nu::FilePath& filePath);

inline FilePath operator/(const FilePath& left, StringView right) {
  return left.append(right);
}

inline FilePath operator/(const FilePath& left, const FilePath& right) {
  return left.append(right);
}

FilePath getCurrentWorkingDirectory();
FilePath getExecutablePath();
bool exists(const FilePath& path);
DynamicArray<FilePath> findAllFilesIn(const FilePath& start);

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_PATH_H_
