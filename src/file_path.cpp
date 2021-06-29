
#include "nucleus/file_path.h"

#include "nucleus/macros.h"
#include "nucleus/text/utils.h"
#if OS(POSIX)
#include <unistd.h>
#if OS(LINUX)
#include <linux/limits.h>
#endif
#elif OS(WIN)
#include "nucleus/Win/includes.h"
#endif

namespace nu {

namespace {

// If this FilePath contains a drive letter specification, returns the position of the last
// character of the drive letter specification, otherwise returns npos.  This can only be true on
// Windows, when a pathname begins with a letter followed by a colon.  On other platforms, this
// always returns npos.
#if OS(WIN)
StringLength findDriveLetter(StringView path) {
  if (path.length() >= 2 && path[1] == ':' &&
      ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
    return 1;
  }

  return StringView::npos;
}
#else
StringLength findDriveLetter(StringView) {
  return StringView::npos;
}
#endif

#if OS(WIN)
bool equalDriveLetterCaseInsensitive(StringView left, StringView right) {
  auto leftLetterPos = findDriveLetter(left);
  auto rightLetterPos = findDriveLetter(right);

  if (leftLetterPos == StringView::npos || rightLetterPos == StringView::npos) {
    return leftLetterPos == rightLetterPos;
  }

#if 0
  auto leftLetter{left.subString(0, leftLetterPos + 1)};
  auto rightLetter{right.subString(0, rightLetterPos + 1)};
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
  for (MemSize i = 0; i < NU_ARRAY_SIZE(kSeparators) - 1; ++i) {
    if (ch == kSeparators[i]) {
      return true;
    }
  }

  return false;
}

// static
FilePath FilePath::normalizeSeparators(StringView path) {
  FilePath result{path};

  for (StringLength i = 0; i < result.m_path.length(); ++i) {
    if (isSeparator(result.m_path[i])) {
      result.m_path[i] = kSeparators[0];
    }
  }

  return result;
}

FilePath::FilePath() = default;

FilePath::FilePath(StringView path) : m_path{path} {}

FilePath::FilePath(const FilePath& other) = default;

FilePath& FilePath::operator=(const FilePath& other) {
  m_path = other.m_path;

  return *this;
}

bool FilePath::operator==(const FilePath& other) const {
#if OS(WIN)
  return equalDriveLetterCaseInsensitive(m_path.view(), other.m_path.view());
#else
  return m_path.view() == other.m_path.view();
#endif
}

bool FilePath::operator!=(const FilePath& other) const {
  return !(*this == other);
}

void FilePath::clear() {}

FilePath FilePath::dirName() const {
  FilePath newPath{*this};

  newPath.stripTrailingSeparators();

  auto letter = findDriveLetter(newPath.m_path.view());

  auto lastSeparator =
      newPath.m_path.view().findLastOfAny(StringView{kSeparators, NU_ARRAY_SIZE(kSeparators) - 1});

  if (lastSeparator == StringView::npos) {
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
  if (newPath.m_path.empty()) {
    newPath.m_path = StringView{kCurrentDirectory, NU_ARRAY_SIZE(kCurrentDirectory) - 1};
  }

  return newPath;
}

FilePath FilePath::baseName() const {
  FilePath newPath = *this;
  newPath.stripTrailingSeparators();

  // The drive letter, if any, is always stripped.
  auto letter = findDriveLetter(newPath.m_path.view());
  if (letter != StringView::npos) {
    newPath.m_path.erase(0, letter + 1);
  }

  // Keep everything after the final separator, but if the pathname is only one character and it's a
  // separator, leave it alone.
  auto lastSeparator =
      newPath.m_path.view().findLastOfAny(StringView{kSeparators, NU_ARRAY_SIZE(kSeparators) - 1});
  if (lastSeparator != StringView::npos && lastSeparator < newPath.m_path.length() - 1) {
    newPath.m_path.erase(0, lastSeparator + 1);
  }

  return newPath;
}

FilePath FilePath::append(StringView component) const {
  return append(FilePath{component});
}

FilePath FilePath::append(const FilePath& component) const {
#if 0
  DCHECK(!isPathAbsolute(*appended));
#endif  // 0

#if 0
  if (m_path.compare(String(kCurrentDirectory, NU_ARRAY_SIZE(kCurrentDirectory) - 1)) == 0 &&
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
  if (!component.m_path.empty() && !newPath.m_path.empty()) {
    // Don't append a separator if the path still ends with a trailing separator after stripping
    // (indicating the root directory).
    if (!isSeparator(newPath.m_path[newPath.m_path.length() - 1])) {
      // Don't append a separator if the path is just a drive letter.
      if (findDriveLetter(newPath.m_path.view()) + 1 != newPath.m_path.length()) {
        newPath.m_path.append(kSeparators[0]);
      }
    }
  }

  newPath.m_path.append(component.m_path.view());

  return newPath;
}

void FilePath::stripTrailingSeparators() {
  // If there is no drive letter, start will be 1, which will prevent stripping the leading
  // separator if there is only one separator.  If there is a drive letter, start will be set
  // appropriately to prevent stripping the first separator following the drive letter, if a
  // separator immediately follows the drive letter.
  auto start = findDriveLetter(m_path.view()) + 2;

  auto lastStripped = StringView::npos;
  for (auto pos = m_path.length(); pos > start && isSeparator(m_path[pos - 1]); --pos) {
    // If the string only has two separators and they're at the beginning, don't strip them, unless
    // the string began with more than two separators.
    if (pos != start + 1 || lastStripped == start + 2 || !isSeparator(m_path[start - 1])) {
      m_path.resize(pos - 1);
      lastStripped = pos;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const FilePath& value) {
  os.rdbuf()->sputn(value.getPath().data(), value.getPath().length());
  return os;
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

FilePath getExecutablePath() {
#if OS(POSIX)
  char buf[PATH_MAX] = {0};
  auto length = readlink("/proc/self/exe", buf, sizeof(buf));
  buf[length] = '\0';
  return FilePath{buf};
#elif OS(WIN)
  char buf[MAX_PATH] = {0};
  auto length = GetModuleFileNameA(GetModuleHandle(nullptr), buf, sizeof(buf));
  DCHECK(length > 0) << "Could not get application executable name.";
  buf[length] = '\0';
  return FilePath{buf};
#else
#error Unsupported operating system.
#endif
}

bool exists(const FilePath& path) {
#if OS(POSIX)
  return access(path.getPath().data(), F_OK) != -1;
#else
  DWORD fileAttributes = GetFileAttributesA(path.getPath().data());
  return fileAttributes != INVALID_FILE_ATTRIBUTES;
#endif
}

#if OS(WIN)
DynamicArray<FilePath> findAllFilesIn(const FilePath& start) {
  LOG(Info) << "List files in directory: " << start;

  DynamicArray<FilePath> result;

  WIN32_FIND_DATA findFileData;
  auto startWithWildcards = start / "*.*";
  HANDLE find = FindFirstFileA(zeroTerminated(startWithWildcards.getPath()).data(), &findFileData);
  if (find == INVALID_HANDLE_VALUE) {
    LOG(Warning) << "Could not list files in directory: " << start.getPath();
    return result;
  }

  for (; FindNextFileA(find, &findFileData);) {
    // Skip `.` and `..`.
    if (findFileData.cFileName[0] == '.') {
      continue;
    }

    result.emplaceBack(start / FilePath{findFileData.cFileName});
  }

  FindClose(find);

  return result;
}
#endif  // OS(WIN)

}  // namespace nu
