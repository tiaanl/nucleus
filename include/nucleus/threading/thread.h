#pragma once

#include "nucleus/Config.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/task.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#elif OS(POSIX)
#include <pthread.h>
#endif

namespace nu {

#if OS(WIN)
using ThreadId = DWORD;
using ThreadHandle = HANDLE;
#elif OS(POSIX)
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

  JoinHandle(JoinHandle&& other) noexcept : handle_{other.handle_} {
    other.handle_ = INVALID_THREAD_HANDLE;
  }

  ~JoinHandle() {
    join();
  }

  JoinHandle& operator=(JoinHandle&& other) noexcept {
    handle_ = other.handle_;
    other.handle_ = INVALID_THREAD_HANDLE;

    return *this;
  }

  void join();

private:
  static constexpr ThreadHandle INVALID_THREAD_HANDLE = std::numeric_limits<ThreadHandle>::max();

  ThreadHandle handle_;
};

NU_NO_DISCARD JoinHandle spawn_thread(ScopedPtr<Task> task);

}  // namespace nu
