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

#include "nucleus/mac/foundation_utils_mac.h"

#import <Foundation/Foundation.h>

namespace nu {
namespace mac {

FilePath NSStringToFilePath(NSString* str) {
  if (![str length])
    return FilePath();
  return FilePath([str fileSystemRepresentation]);
}

bool getSearchPathDirectory(NSSearchPathDirectory directory,
                            NSSearchPathDomainMask domainMask,
                            FilePath* result) {
  DCHECK(result);
  NSArray* dirs =
      NSSearchPathForDirectoriesInDomains(directory, domainMask, YES);
  if ([dirs count] < 1) {
    return false;
  }
  *result = NSStringToFilePath([dirs objectAtIndex:0]);
  return true;
}

bool getUserDirectory(NSSearchPathDirectory directory, FilePath* result) {
  return GetSearchPathDirectory(directory, NSUserDomainMask, result);
}

FilePath getUserLibraryPath() {
  FilePath userLibraryPath;
  if (!getUserDirectory(NSLibraryDirectory, &userLibraryPath)) {
    DLOG(Warning) << "Could not get user library path";
  }
  return userLibraryPath;
}

}  // namespace mac
}  // namespace nu
