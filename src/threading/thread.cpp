#include "nucleus/threading/thread.h"

#if OS(POSIX)
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace nu {

namespace {

struct ThreadContext {
  Function<void()> function;
};

// static
#if OS(WIN)
DWORD WINAPI thread_main(void* c) {
#elif OS(POSIX)
void* thread_main(void* c) {
#endif
  auto thread = reinterpret_cast<ThreadContext*>(c);
  thread->function();
  thread->function.reset();
#if OS(WIN)
  return 0;
#elif OS(POSIX)
  return nullptr;
#endif
}

}  // namespace

namespace current_thread {

// static
ThreadId id() {
#if OS(WIN)
  return GetCurrentThreadId();
#elif OS(POSIX)
  return syscall(__NR_gettid);
#endif
}

}  // namespace current_thread

void JoinHandle::join() {
  if (handle_ == INVALID_THREAD_HANDLE) {
    return;
  }
#if OS(WIN)
  WaitForSingleObject(handle_, INFINITE);
#elif OS(POSIX)
  void* r;
  pthread_join(handle_, &r);
#endif

  // When the thread exit's, the thread handle is not valid any more.
  handle_ = INVALID_THREAD_HANDLE;
}

JoinHandle spawn_thread(Function<void()> function) {
  auto c = make_scoped_ptr<ThreadContext>(std::move(function));
  ThreadHandle handle;
#if OS(WIN)
  handle = CreateThread(nullptr, 0, &thread_main, c.release(), 0, nullptr);
#elif OS(POSIX)
  pthread_create(&handle, nullptr, thread_main, c.release());
#endif
  return JoinHandle{handle};
}

}  // namespace nu
