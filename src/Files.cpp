
#include "nucleus/Config.h"
#include "nucleus/FilePath.h"

#if OS(POSIX)
#include <unistd.h>
#elif OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#endif

namespace nu {

FilePath getCurrentWorkingDirectory(Allocator* allocator) {
#if OS(POSIX)
  char buf[PATH_MAX] = {0};
  const char* result = ::getcwd(buf, PATH_MAX);
  return FilePath{String{result, String::npos, allocator}, allocator};
#elif OS(WIN)
  char buf[MAX_PATH] = {0};
  DWORD result = ::GetCurrentDirectoryA(MAX_PATH, buf);
  return FilePath{String{buf, result, allocator}, allocator};
#endif
}

}  // namespace nu
