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

#include <mach-o/dyld.h>

#include <vector>

#include "nucleus/mac/foundation_utils_mac.h"
#include "nucleus/utils/stl.h"

namespace nu {

bool SpecialPath::getExePath(FilePath* pathOut) {
  uint32_t size = 0;
  _NSGetExecutablePath((char*)0, &size);
  DCHECK(size > 1);

  std::vector<char> buffer;
  _NSGetExecutablePath(VectorAsArray(&buffer, size), &size);

  *pathOut = FilePath::StringType(buffer.data());

  return true;
}

bool SpecialPath::getConfigRootDir(FilePath* pathOut) {
  FilePath path =
      mac::getUserLibraryPath().Append(STRING_LITERAL("Preferences"));

  *pathOut = path;

  return true;
}

}  // namespace nu
