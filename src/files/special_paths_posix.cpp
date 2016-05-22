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


#include "nuclues/files/special_paths.h"

#include <limits.h>
#include <string.h>
#include <unistd.h>

namespace nu {

#if !OS(ANDROID)
#if 0
// static
bool SpecialPath::getExePath(FilePath* pathOut) {
  return false;

  char path[PATH_MAX];
  char* path_end;

  // Read the target of /proc/self/exe.
  if (readlink ("/proc/self/exe", path, PATH_MAX) <= 0)
    return false;

  path_end = strrchr (path, '/');
  if (path_end == NULL)
    return false;

  // Advance to the character past the last slash.
  ++path_end;

  // Add null terminator to end the path before the
  // executable name.
  *path_end = '\0';

  *pathOut = FilePath(path);
  LOG(Info) << "Executable path : " << *pathOut;

  return true;
}
#endif  // 0

// static
bool SpecialPath::getTempDirectory(FilePath* pathOut) {
  const char* path = getenv("TMPDIR");
  if (path) {
    *pathOut = FilePath(path);
    return true;
  }

  LOG(Info) << strerror(errno);
  return false;
}

#if OS(LINUX)
// static
bool SpecialPath::getConfigRootDir(FilePath* pathOut) {
  LOG(Info) << "Calling posix GetConfigRootDir function!";
  const char* path = getenv("HOME");
  if (path) {
    *pathOut = FilePath(path);
    return true;
  }

  LOG(Info) << strerror(errno);

  return false;
}
#endif  // OS(LINUX)

#endif  // !OS(ANDROID)

// static
bool SpecialPath::getSourceRoot(FilePath* pathOut) {
  FilePath root(__FILE__);

  root = root.dirName().dirName().dirName().dirName();
  *pathOut = root;

  return true;
}

}  // namespace nu
