
#ifndef NUCLEUS_UTILS_FILES_H_
#define NUCLEUS_UTILS_FILES_H_

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/Files/FilePath.h"

namespace nu {

FilePath getCurrentWorkingDirectory(Allocator* allocator = getDefaultAllocator());

}  // namespace nu

#endif  // NUCLEUS_UTILS_FILES_H_