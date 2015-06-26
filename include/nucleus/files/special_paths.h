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

#ifndef NUCLEUS_FILES_SPECIAL_PATH_H_
#define NUCLEUS_FILES_SPECIAL_PATH_H_


#include "nucleus/files/file_path.h"
#include "nucleus/macros.h"

namespace nu {

class SpecialPath {
public:
  enum Path {
    // Executable location.
    EXE_PATH,

    // The source tree root directory.
    SOURCE_ROOT,

    // For Android - the cache directory
    CACHE_DIRECTORY,

    // The config file directory
    CONFIG_DIRECTORY,

    // The platform's temporary directory.
    TEMP_DIRECTORY,
    PATH_COUNT  // Sentinel
  };

  static bool get(Path path, FilePath* pathOut);

private:
#if OS(ANDROID)
  static bool getCachePath(FilePath* pathOut);
#endif

  // Get the executable directory.
  static bool getExePath(FilePath* pathOut);

  // Get the root directory where user configuration is stored.
  static bool getConfigRootDir(FilePath* pathOut);

  // Internal platform specific function to get the temporary directory.
  static bool getTempDirectory(FilePath* pathOut);

  // Set the source root into |pathOut|.
  static bool getSourceRoot(FilePath* pathOut);

  DISALLOW_IMPLICIT_CONSTRUCTORS(SpecialPath);
};

}  // namespace nu

#endif  // NUCLEUS_FILES_SPECIAL_PATH_H_
