
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
#if OS(POSIX)
  // On most posix platforms, native pathnames are char arrays, and the encoding
  // may or may not be specified.  On Mac OS X, native pathnames are encoded in
  // UTF-8.
  using StringType = String;
#else
  // On Windows, native pathnames are wchar_t arrays encoded in UTF-16.
  using StringType = String;
#endif

  using CharType = StringType::CharType;

  // Null-terminated array of separators used to separate components in hierarchical paths.  Each character in this
  // array is a valid separator, but kSeparators[0] is treated as the canonical separator and will be used when
  // composing path names.
  static const CharType kSeparators[];

  // A special path component meaning "this directory."
  static const CharType kCurrentDirectory[];

  // A special path component meaning "the parent directory."
  static const CharType kParentDirectory[];

  // The character used to identify a file extension.
  static const CharType kExtensionSeparator;

  // Returns true if ch is a FilePath separator.
  static bool isSeparator(CharType ch);

  // Normalize separators to all be the default separator for the platform.
  static FilePath normalizeSeparators(const nu::String& path, Allocator* allocator = getDefaultAllocator());

  explicit FilePath(Allocator* allocator = getDefaultAllocator());
  explicit FilePath(const StringType& path, Allocator* allocator = getDefaultAllocator());

  FilePath(const FilePath& other, Allocator* allocator = getDefaultAllocator());

  ~FilePath();

  FilePath& operator=(const FilePath& other);

  bool operator==(const FilePath& other) const;
  bool operator!=(const FilePath& other) const;

  // Return the path as a string.
  const StringType& getPath() const {
    return m_path;
  }

  // Clear the path.
  void clear();

  // Returns a FilePath corresponding to the directory containing the path named by this object, stripping away the file
  // component.
  FilePath dirName() const;

  // Returns a FilePath corresponding to the last path component of this object, either a file or a directory.
  FilePath baseName() const;

  // Returns a FilePath by appending a separator and the supplied path component to this object's path.  Append takes
  // care to avoid adding excessive separators if this object's path already ends with a separator.
  FilePath append(const StringType& component) const;
  FilePath append(const FilePath& component) const;

private:
  // Remove trailing separators from this object.
  void stripTrailingSeparators();

  Allocator* m_allocator;

  // A string representation of the path in this object.
  StringType m_path;
};

// Macro for string literal initialization of FilePath::CharType[].
//#if OS(POSIX)
#define FILE_PATH_LITERAL(Str) Str
//#elif OS(WIN)
//#define FILE_PATH_LITERAL(Str) L##Str
//#endif

inline std::ostream& operator<<(std::ostream& os, const FilePath& filePath) {
  os << filePath.getPath();
  return os;
}

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_PATH_H_
