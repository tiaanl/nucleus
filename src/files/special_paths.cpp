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

#include "nucleus/files/special_paths.h"
#include "nucleus/logging.h"
#include "nucleus/win/windows_mixin.h"

namespace nu {

// static
bool SpecialPath::get(Path path, FilePath* pathOut) {
  DCHECK(static_cast<int>(path) >= 0 && static_cast<int>(path) < PATH_COUNT)
      << "Invalid path specified.";

  switch (path) {
    case EXE_PATH:
      return getExePath(pathOut);

    case TEMP_DIRECTORY:
      return getTempDirectory(pathOut);

    case SOURCE_ROOT:
      return getSourceRoot(pathOut);

    case CONFIG_DIRECTORY:
      return getConfigRootDir(pathOut);

    default:
      NOTREACHED() << "Invalid special path key.";
      break;
  }

  return false;
}

}  // namespace nu
