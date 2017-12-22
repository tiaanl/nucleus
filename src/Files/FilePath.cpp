
#include "nucleus/Files/FilePath.h"

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace {

using StringType = FilePath::StringType;

// If this FilePath contains a drive letter specification, returns the position of the last character of the drive
// letter specification, otherwise returns npos.  This can only be true on Windows, when a pathname begins with a letter
// followed by a colon.  On other platforms, this always returns npos.
StringType::SizeType findDriveLetter(const StringType& path) {
#if OS(WIN)
  if (path.getLength() >= 2 && path[1] == ':' &&
      ((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z'))) {
    return 1;
  }
#endif
  return StringType::npos;
}

#if OS(WIN)
bool equalDriveLetterCaseInsensitive(const StringType& left, const StringType& right) {
  auto leftLetterPos = findDriveLetter(left);
  auto rightLetterPos = findDriveLetter(right);

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

#if OS(WIN)
const FilePath::CharType FilePath::kSeparators[] = FILE_PATH_LITERAL("\\/");
#else
const FilePath::CharType FilePath::kSeparators[] = FILE_PATH_LITERAL("/");
#endif

const FilePath::CharType FilePath::kCurrentDirectory[] = FILE_PATH_LITERAL(".");
const FilePath::CharType FilePath::kParentDirectory[] = FILE_PATH_LITERAL("..");

const FilePath::CharType FilePath::kExtensionSeparator = FILE_PATH_LITERAL('.');

// static
bool FilePath::isSeparator(CharType ch) {
  for (USize i = 0; i < ARRAY_SIZE(kSeparators) - 1; ++i) {
    if (ch == kSeparators[i]) {
      return true;
    }
  }

  return false;
}

FilePath::FilePath() = default;

FilePath::FilePath(const FilePath& other) = default;

FilePath::FilePath(const StringType& path) : m_path(path) {}

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

  StringType::SizeType letter = findDriveLetter(newPath.m_path);

  StringType::SizeType lastSeparator =
      newPath.m_path.findLastOfAnyChar(String::fromCString(kSeparators, ARRAY_SIZE(kSeparators)));
  if (lastSeparator == StringType::npos) {
    // m_path is in the current directory.
    newPath.m_path.resize(letter + 1);
  } else if (lastSeparator == letter + 1) {
    // m_path is in the root directory.
    newPath.m_path.resize(letter + 2);
  } else if (lastSeparator == letter + 2 && isSeparator(newPath.m_path.at(letter + 1))) {
    // m_path is in "//" (possibly with a drive letter), so leave the double
    // separator intact indicating alternate root.
    newPath.m_path.resize(letter + 3);
  } else if (lastSeparator != 0) {
    // m_path is somewhere else, trim the base name.
    newPath.m_path.resize(lastSeparator);
  }

  newPath.stripTrailingSeparators();
  if (newPath.m_path.isEmpty()) {
    newPath.m_path = String::fromCString(kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory));
  }

  return newPath;
}

FilePath FilePath::baseName() const {
  FilePath newPath(m_path);
  newPath.stripTrailingSeparators();

  // The drive letter, if any, is always stripped.
  StringType::SizeType letter = findDriveLetter(newPath.m_path);
  if (letter != StringType::npos) {
    newPath.m_path.erase(0, letter + 1);
  }

  // Keep everything after the final separator, but if the pathname is only one
  // character and it's a separator, leave it alone.
  StringType::SizeType lastSeparator =
      newPath.m_path.findLastOfAnyChar(String::fromCString(kSeparators, ARRAY_SIZE(kSeparators)));
  if (lastSeparator != StringType::npos && lastSeparator < newPath.m_path.getLength() - 1) {
    newPath.m_path.erase(0, lastSeparator + 1);
  }

  return newPath;
}

FilePath FilePath::append(const StringType& component) const {
  const StringType* appended = &component;
  StringType withoutNulls;

  StringType::SizeType nulPos = component.find(FILE_PATH_LITERAL('\0'));
  if (nulPos != StringType::npos) {
    withoutNulls = component.sub(0, nulPos);
    appended = &withoutNulls;
  }

#if 0
  DCHECK(!isPathAbsolute(*appended));
#endif  // 0

  if (m_path.compare(String::fromCString(kCurrentDirectory, ARRAY_SIZE(kCurrentDirectory))) == 0) {
    // Append normally doesn't do any normalization, but as a special case, when
    // appending to kCurrentDirectory, just return a new path for the component
    // argument.  Appending component to kCurrentDirectory would serve no
    // purpose other than needlessly lengthening the path, and it's likely in
    // practice to wind up with FilePath objects containing only
    // kCurrentDirectory when calling DirName on a single relative path
    // component.
    return FilePath(*appended);
  }

  FilePath newPath(m_path);
  newPath.stripTrailingSeparators();

  // Don't append a separator if the path is empty (indicating the current
  // directory) or if the path component is empty (indicating nothing to
  // append).
  if (!appended->isEmpty() && !newPath.m_path.isEmpty()) {
    // Don't append a separator if the path still ends with a trailing separator
    // after stripping (indicating the root directory).
    if (!isSeparator(newPath.m_path.at(newPath.m_path.getLength() - 1))) {
      // Don't append a separator if the path is just a drive letter.
      if (findDriveLetter(newPath.m_path) + 1 != newPath.m_path.getLength()) {
        newPath.m_path.append(kSeparators[0]);
      }
    }
  }

  newPath.m_path.append(*appended);

  return newPath;
}

FilePath FilePath::append(const FilePath& component) const {
  return append(component.getPath());
}

void FilePath::stripTrailingSeparators() {
  // If there is no drive letter, start will be 1, which will prevent stripping
  // the leading separator if there is only one separator.  If there is a drive
  // letter, start will be set appropriately to prevent stripping the first
  // separator following the drive letter, if a separator immediately follows
  // the drive letter.
  StringType::SizeType start = findDriveLetter(m_path) + 2;

  StringType::SizeType lastStripped = StringType::npos;
  for (StringType::SizeType pos = m_path.getLength(); pos > start && isSeparator(m_path.at(pos - 1)); --pos) {
    // If the string only has two separators and they're at the beginning, don't
    // strip them, unless the string began with more than two separators.
    if (pos != start + 1 || lastStripped == start + 2 || !isSeparator(m_path.at(start - 1))) {
      m_path.resize(pos - 1);
      lastStripped = pos;
    }
  }
}

}  // namespace nu
