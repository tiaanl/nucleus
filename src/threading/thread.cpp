#include "nucleus/threading/thread.h"

#if OS(POSIX)
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace nu {

namespace {

#if OS(POSIX)

struct ThreadContext {
  ScopedPtr<Task> task;
};

// static
void* thread_main(void* c) {
  auto thread = reinterpret_cast<ThreadContext*>(c);
  thread->task->execute();
  thread->task.reset();
  return nullptr;
}
#endif  // OS(POSIX)

}  // namespace

namespace current_thread {

// static
ThreadId id() {
  return syscall(__NR_gettid);
}

}  // namespace current_thread

void JoinHandle::join() {
  void* r;
  pthread_join(handle_, &r);
}

JoinHandle::~JoinHandle() {
  join();
}

JoinHandle spawn_thread(ScopedPtr<Task> task) {
  auto c = makeScopedPtr<ThreadContext>(std::move(task));
  pthread_t handle;
  pthread_create(&handle, nullptr, thread_main, c.release());
  return JoinHandle{handle};
}

}  // namespace nu
