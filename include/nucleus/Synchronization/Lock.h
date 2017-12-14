
#ifndef NUCLEUS_SYNCHRONIZATION_LOCK_H_
#define NUCLEUS_SYNCHRONIZATION_LOCK_H_

#include "nucleus/Config.h"
#include "nucleus/Macros.h"

#if OS(WIN)
#include "nucleus/win/windows_mixin.h"
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

  void Acquire();
  void Release();

private:
  DISALLOW_COPY_AND_ASSIGN(Lock);
};

}  // namespace nu

#endif  // NUCLEUS_SYNCHRONIZATION_H_
