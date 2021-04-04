#pragma once

#include "nucleus/Config.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/task.h"

#if OS(POSIX)
#include <pthread.h>
#endif

namespace nu {

#if OS(POSIX)
using ThreadId = pid_t;
using ThreadHandle = pthread_t;
#endif

namespace current_thread {

ThreadId id();

}  // namespace current_thread

class JoinHandle {
  NU_DELETE_COPY(JoinHandle);

public:
  explicit JoinHandle(ThreadHandle handle) : handle_{handle} {}
  ~JoinHandle();

  void join();

private:
  ThreadHandle handle_;
};

NU_NO_DISCARD JoinHandle spawn_thread(ScopedPtr<Task> task);

}  // namespace nu
