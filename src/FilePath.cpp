
#include "nucleus/FilePath.h"

#include "nucleus/Macros.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace {

// If this FilePath contains a drive letter specification, returns the position of the last character of the drive
// letter specification, otherwise returns npos.  This can only be true on Windows, when a pathname begins with a letter
// followed by a colon.  On other platforms, this always returns npos.
#if OS(WIN)
String::SizeType findDriveLetter(const String& path) {
  if (path.getLength() >= 2 && path[1] == ':' &&
      ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
    return 1;
  }
  return String::npos;
}
#else
String::SizeType findDriveLetter(const String&) {
  return String::npos;
}
#endif

#if OS(WIN)
bool equalDriveLetterCaseInsensitive(const String& left, const String& right) {
  auto leftLetterPos = findDriveLetter(left);
  auto rightLetterPos = findDriveLetter(right);

  if (leftLetterPos == String::npos || rightLetterPos == String::npos) {
    return leftLetterPos == rightLetterPos;
  }

  String leftLetter{left.sub(0, leftLetterPos + 1)};
  String rightLetter{right.sub(0, rightLetterPos + 1)};
#if 0
  if (!StartsWith(leftLetter, rightLetter, false))
    return false;
#endif  // 0

  String leftRest{left.sub(leftLetterPos + 1)};
  String rightRest{right.sub(rightLetterPos + 1)};

  return leftRest == rightRest;
}
#endif  // OS(WIN)

// Null-terminated array of separators used to separate components in hierarchical paths.  Each character in this array
// is a valid separator, but kSeparators[0] is treated as the canonical separator and will be used when composing path
// names.
#if OS(WIN)
static const char kSeparators[] = "\\/";
#else
static const char kSeparators[] = "/";
#endif

// A special path component meaning "this directory."
static const char kCurrentDirectory[] = ".";

// A special path component meaning "the parent directory."
static const char kParentDirectory[] = "..";

// The character used to identify a file extension.
static const char kExtensionSeparator = '.';

}  // namespace

// static
bool FilePath::isSeparator(String::CharType ch) {
  for (MemSize i = 0; i < ARRAY_SIZE(kSeparators) - 1; ++i) {
    if (ch == kSeparators[i]) {
      return true;
    }
  }

  return false;
}

// static
FilePath FilePath::normalizeSeparators(const nu::String& path) {
  FilePath result{path};
  for (String::SizeType i = 0; i < result.m_path.getLength(); ++i) {
    String::CharType ch = result.m_path[i];
    if (isSeparator(ch)) {
      result.m_path[i] = kSeparators[0];
    }
  }
  return result;
}

FilePath::FilePath() = default;

FilePath::FilePath(const String& path) : m_path(path) {}

FilePath::FilePath(const FilePath& other) : m_path{other.m_path} {}

FilePath::~FilePath() = default;

FilePath& FilePath::operator=(const FilePath& other) = default;

bool FilePath::operator==(const FilePath& other) const {
#if OS(WIN)
  return equalDriveLetterCaseInsensitive(m_path, other.m_path);
#else
  return m_path == other.m_path;
#endif
}

bool FilePath::operator!=(const FilePath& other) const {
  return !(*this == other);
}

void FilePath::clear() {}

FilePath FilePath::dirName() const {
  FilePath newPath{m_path};
  newPath.stripTrailingSeparators();

  String::SizeType letter = findDriveLetter(newPath.m_path);

  String::SizeType lastSeparator =
      newPath.m_path.findLastOfAnyChar(String{kSeparators, ARRAY_SIZE(kSeparators) - 1});
  if (lastSeparator == String::npos) {
    // m_path is in the current directory.
    newPath.m_path.resize(letter + 1);
  } else if (lastSeparator == letter + 1) {
    // m_path is in the root directory.
    newPath.m_path.resize(letter + 2);
  } else if (lastSeparator == letter + 2 && isSeparator(newPath.m_path[letter + 1])) {
    // m_path is in "//" (possibly with a drive letter), so leave the double separator intact indicating alternate root.
    newPath.m_path.resize(letter + 3);
  } else if (lastSeparator != 0) {
    // m_path is somewhere else, trim the base name.
    newPath.m_path.resize(lastSeparator);
  }

  newPath.stripTrailingSeparators();
  if (newPath.m_path.isEmpty()) {
    newPath.m_path = String(kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory) - 1);
  }

  return newPath;
}

FilePath FilePath::baseName() const {
  FilePath newPath{m_path};
  newPath.stripTrailingSeparators();

  // The drive letter, if any, is always stripped.
  String::SizeType letter = findDriveLetter(newPath.m_path);
  if (letter != String::npos) {
    newPath.m_path.erase(0, letter + 1);
  }

  // Keep everything after the final separator, but if the pathname is only one character and it's a separator, leave it
  // alone.
  String::SizeType lastSeparator =
      newPath.m_path.findLastOfAnyChar(String(kSeparators, ARRAY_SIZE(kSeparators) - 1));
  if (lastSeparator != String::npos && lastSeparator < newPath.m_path.getLength() - 1) {
    newPath.m_path.erase(0, lastSeparator + 1);
  }

  return newPath;
}

FilePath FilePath::append(const char* component) const {
  return append(String{component});
}

FilePath FilePath::append(const String& component) const {
#if 0
  DCHECK(!isPathAbsolute(*appended));
#endif  // 0

  if (m_path.compare(String(kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory) - 1)) == 0 &&
      !component.isEmpty()) {
    // Append normally doesn't do any normalization, but as a special case, when appending to `kCurrentDirectory`, just
    // return a new path for the `component` argument.  Appending `component` to `kCurrentDirectory` would serve no
    // purpose other than needlessly lengthening the path.
    return FilePath(component);
  }

  FilePath newPath{m_path};
  newPath.stripTrailingSeparators();

  // Don't append a separator if the path is empty (indicating the current directory) or if the path component is empty
  // (indicating nothing to append).
  if (!component.isEmpty() && !newPath.m_path.isEmpty()) {
    // Don't append a separator if the path still ends with a trailing separator after stripping (indicating the root
    // directory).
    if (!isSeparator(newPath.m_path[newPath.m_path.getLength() - 1])) {
      // Don't append a separator if the path is just a drive letter.
      if (findDriveLetter(newPath.m_path) + 1 != newPath.m_path.getLength()) {
        newPath.m_path.append(kSeparators[0]);
      }
    }
  }

  newPath.m_path.append(component);

  return newPath;
}

FilePath FilePath::append(const FilePath& component) const {
  return append(component.getPath());
}

void FilePath::stripTrailingSeparators() {
  // If there is no drive letter, start will be 1, which will prevent stripping the leading separator if there is only
  // one separator.  If there is a drive letter, start will be set appropriately to prevent stripping the first
  // separator following the drive letter, if a separator immediately follows the drive letter.
  String::SizeType start = findDriveLetter(m_path) + 2;

  String::SizeType lastStripped = String::npos;
  for (String::SizeType pos = m_path.getLength(); pos > start && isSeparator(m_path[pos - 1]); --pos) {
    // If the string only has two separators and they're at the beginning, don't strip them, unless the string began
    // with more than two separators.
    if (pos != start + 1 || lastStripped == start + 2 || !isSeparator(m_path[start - 1])) {
      m_path.resize(pos - 1);
      lastStripped = pos;
    }
  }
}

}  // namespace nu
