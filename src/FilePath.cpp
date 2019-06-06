
#include "nucleus/FilePath.h"

#include "nucleus/Macros.h"
#if OS(POSIX)
#include <unistd.h>
#if OS(LINUX)
#include <linux/limits.h>
#endif
#elif OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#endif

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace {

// If this FilePath contains a drive letter specification, returns the position of the last
// character of the drive letter specification, otherwise returns npos.  This can only be true on
// Windows, when a pathname begins with a letter followed by a colon.  On other platforms, this
// always returns npos.
#if OS(WIN)
StringLength findDriveLetter(const StringView& path) {
  if (path.getLength() >= 2 && path[1] == ':' &&
      ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
    return 1;
  }

  return StringView::kInvalidPosition;
}
#else
String::SizeType findDriveLetter(const String&) {
  return String::npos;
}
#endif

#if OS(WIN)
bool equalDriveLetterCaseInsensitive(const StringView& left, const StringView& right) {
  auto leftLetterPos = findDriveLetter(left);
  auto rightLetterPos = findDriveLetter(right);

  if (leftLetterPos == StringView::kInvalidPosition ||
      rightLetterPos == StringView::kInvalidPosition) {
    return leftLetterPos == rightLetterPos;
  }

  auto leftLetter{left.subString(0, leftLetterPos + 1)};
  auto rightLetter{right.subString(0, rightLetterPos + 1)};
#if 0
  if (!StartsWith(leftLetter, rightLetter, false))
    return false;
#endif  // 0

  auto leftRest = left.subString(leftLetterPos + 1);
  auto rightRest = left.subString(rightLetterPos + 1);

  return leftRest == rightRest;
}
#endif  // OS(WIN)

// Null-terminated array of separators used to separate components in hierarchical paths.  Each
// character in this array is a valid separator, but kSeparators[0] is treated as the canonical
// separator and will be used when composing path names.
#if OS(WIN)
static const char kSeparators[] = "\\/";
#else
static const char kSeparators[] = "/";
#endif

// A special path component meaning "this directory."
static const char kCurrentDirectory[] = ".";

#if 0
// A special path component meaning "the parent directory."
static const char kParentDirectory[] = "..";
#endif

#if 0
// The character used to identify a file extension.
static const char kExtensionSeparator = '.';
#endif

}  // namespace

// static
bool FilePath::isSeparator(Char ch) {
  for (MemSize i = 0; i < ARRAY_SIZE(kSeparators) - 1; ++i) {
    if (ch == kSeparators[i]) {
      return true;
    }
  }

  return false;
}

// static
FilePath FilePath::normalizeSeparators(const StringView& path) {
  FilePath result{path};

  for (StringLength i = 0; i < result.m_path.getLength(); ++i) {
    if (isSeparator(result.m_path[i])) {
      result.m_path[i] = kSeparators[0];
    }
  }

  return result;
}

FilePath::FilePath() = default;

FilePath::FilePath(const StringView& path) : m_path{path.getData(), path.getLength()} {}

FilePath::FilePath(const FilePath& other) = default;

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
  FilePath newPath{*this};

  newPath.stripTrailingSeparators();

  auto letter = findDriveLetter(newPath.m_path);

  auto lastSeparator =
      newPath.m_path.findLastOfAny(StringView{kSeparators, ARRAY_SIZE(kSeparators) - 1});

  if (lastSeparator == StringView::kInvalidPosition) {
    // m_path is in the current directory.
    newPath.m_path.resize(letter + 1);
  } else if (lastSeparator == letter + 1) {
    // m_path is in the root directory.
    newPath.m_path.resize(letter + 2);
  } else if (lastSeparator == letter + 2 && isSeparator(newPath.m_path[letter + 1])) {
    // m_path is in "//" (possibly with a drive letter), so leave the double separator intact
    // indicating alternate root.
    newPath.m_path.resize(letter + 3);
  } else if (lastSeparator != 0) {
    // m_path is somewhere else, trim the base name.
    newPath.m_path.resize(lastSeparator);
  }

  newPath.stripTrailingSeparators();
  if (newPath.m_path.isEmpty()) {
    newPath.m_path = StringView{kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory) - 1};
  }

  return newPath;
}

FilePath FilePath::baseName() const {
  FilePath newPath = *this;
  newPath.stripTrailingSeparators();

  // The drive letter, if any, is always stripped.
  auto letter = findDriveLetter(newPath.m_path);
  if (letter != StringView::kInvalidPosition) {
    newPath.m_path.erase(0, letter + 1);
  }

  // Keep everything after the final separator, but if the pathname is only one character and it's a
  // separator, leave it alone.
  auto lastSeparator =
      newPath.m_path.findLastOfAny(StringView{kSeparators, ARRAY_SIZE(kSeparators) - 1});
  if (lastSeparator != StringView::kInvalidPosition &&
      lastSeparator < newPath.m_path.getLength() - 1) {
    newPath.m_path.erase(0, lastSeparator + 1);
  }

  return newPath;
}

FilePath FilePath::append(const StringView& component) const {
  return append(FilePath{component});
}

FilePath FilePath::append(const FilePath& component) const {
#if 0
  DCHECK(!isPathAbsolute(*appended));
#endif  // 0

#if 0
  if (m_path.compare(String(kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory) - 1)) == 0 &&
      !component.isEmpty()) {
    // Append normally doesn't do any normalization, but as a special case, when appending to
    // `kCurrentDirectory`, just return a new path for the `component` argument.  Appending
    // `component` to `kCurrentDirectory` would serve no purpose other than needlessly lengthening
    // the path.
    return FilePath{component};
  }
#endif  // 0

  FilePath newPath = *this;
  newPath.stripTrailingSeparators();

  // Don't append a separator if the path is empty (indicating the current directory) or if the path
  // component is empty (indicating nothing to append).
  if (!component.m_path.isEmpty() && !newPath.m_path.isEmpty()) {
    // Don't append a separator if the path still ends with a trailing separator after stripping
    // (indicating the root directory).
    if (!isSeparator(newPath.m_path[newPath.m_path.getLength() - 1])) {
      // Don't append a separator if the path is just a drive letter.
      if (findDriveLetter(newPath.m_path) + 1 != newPath.m_path.getLength()) {
        newPath.m_path.append(kSeparators[0]);
      }
    }
  }

  newPath.m_path.append(component.m_path);

  return newPath;
}

void FilePath::stripTrailingSeparators() {
  // If there is no drive letter, start will be 1, which will prevent stripping the leading
  // separator if there is only one separator.  If there is a drive letter, start will be set
  // appropriately to prevent stripping the first separator following the drive letter, if a
  // separator immediately follows the drive letter.
  auto start = findDriveLetter(m_path) + 2;

  auto lastStripped = StringView::kInvalidPosition;
  for (auto pos = m_path.getLength(); pos > start && isSeparator(m_path[pos - 1]); --pos) {
    // If the string only has two separators and they're at the beginning, don't strip them, unless
    // the string began with more than two separators.
    if (pos != start + 1 || lastStripped == start + 2 || !isSeparator(m_path[start - 1])) {
      m_path.resize(pos - 1);
      lastStripped = pos;
    }
  }
}

FilePath getCurrentWorkingDirectory() {
#if OS(POSIX)
  char buf[PATH_MAX] = {0};
  const char* result = ::getcwd(buf, PATH_MAX);
  return FilePath{result};
#elif OS(WIN)
  char buf[MAX_PATH] = {0};
  DWORD result = ::GetCurrentDirectoryA(MAX_PATH, buf);
  return FilePath{StringView{buf, result}};
#endif
}

}  // namespace nu
