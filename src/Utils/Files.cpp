
#include "nucleus/Config.h"
#include "nucleus/Files/FilePath.h"

#if OS(POSIX)
#include <unistd.h>
#endif

namespace nu {

#if OS(POSIX)
FilePath getCurrentWorkingDirectory(Allocator* allocator) {
  char buf[PATH_MAX] = {0};
  const char* result = ::getcwd(buf, PATH_MAX);
  return FilePath{String{result, String::npos, allocator}, allocator};
}
#endif

}  // namespace nu
