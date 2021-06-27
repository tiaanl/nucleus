#pragma once

#include "nucleus/config.h"
#include "nucleus/macros.h"

#if OS(WIN)
#include "nucleus/Win/includes.h"
#elif OS(POSIX)
#include <pthread.h>
#endif

namespace nu {

class Lock {
public:
#if OS(WIN)
  typedef CRITICAL_SECTION NativeHandle;
#elif OS(POSIX)
  typedef pthread_mutex_t NativeHandle;
#endif

  Lock();
  ~Lock();

  void acquire();
  void release();

private:
  NU_DELETE_COPY_AND_MOVE(Lock);
};

}  // namespace nu
