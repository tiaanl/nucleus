
#ifndef NUCLEUS_FILES_FILE_PATH_H_
#define NUCLEUS_FILES_FILE_PATH_H_

#include <ostream>

#include "nucleus/Config.h"
#include "nucleus/Text/String.h"

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
  static bool isSeparator(String::CharType ch);

  // Normalize separators to all be the default separator for the platform.
  static FilePath normalizeSeparators(const nu::String& path);

  FilePath();
  explicit FilePath(const String& path);
  FilePath(const FilePath& other);
  ~FilePath();

  FilePath& operator=(const FilePath& other);

  bool operator==(const FilePath& other) const;
  bool operator!=(const FilePath& other) const;

  // Return the path as a string.
  const String& getPath() const {
    return m_path;
  }

  // Clear the path.
  void clear();

  // Returns a FilePath corresponding to the directory containing the path named by this object,
  // stripping away the file component.
  FilePath dirName() const;

  // Returns a FilePath corresponding to the last path component of this object, either a file or a
  // directory.
  FilePath baseName() const;

  // Returns a FilePath by appending a separator and the supplied path component to this object's
  // path.  Append takes care to avoid adding excessive separators if this object's path already
  // ends with a separator.
  FilePath append(const char* component) const;
  FilePath append(const String& component) const;
  FilePath append(const FilePath& component) const;

private:
  // Remove trailing separators from this object.
  void stripTrailingSeparators();

  // A string representation of the path in this object.
  String m_path;
};

inline std::ostream& operator<<(std::ostream& os, const FilePath& filePath) {
  os << filePath.getPath();
  return os;
}

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_PATH_H_
