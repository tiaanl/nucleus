
#ifndef NUCLEUS_FILES_FILE_PATH_H_
#define NUCLEUS_FILES_FILE_PATH_H_

#include <functional>
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
  const StringType& getPath() const {
    return m_path;
  }

  // Return true if the path is empty.
  bool isEmpty() const {
    return m_path.empty();
  }

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

// Provide a std::hash for a FilePath.
namespace std {

template <>
struct hash<nu::FilePath> {
  std::size_t operator()(const nu::FilePath& path) const {
    using std::hash;

    hash<nu::FilePath::StringType> hashFunc;
    return hashFunc(path.getPath());
  }
};

}  // namespace std

#endif  // NUCLEUS_FILES_FILE_PATH_H_
