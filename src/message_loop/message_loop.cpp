#include "nucleus/message_loop/message_loop.h"

namespace nu {

MessageLoop::MessageLoop() = default;

void MessageLoop::post_task(ScopedPtr<Task> task) {
  queue_.emplace_back(std::move(task));
}

void MessageLoop::post_quit() {
  post_task(makeScopedPtr<QuitTask>(this));
}

void MessageLoop::run_until_idle() {
  quit_on_idle_ = true;
  run_internal();
}

void MessageLoop::run() {
  run_internal();
}

void MessageLoop::run_internal() {
  while (!queue_.empty()) {
    auto task_to_execute = std::move(queue_.front());
    queue_.pop_front();

    task_to_execute->execute();
  }
}

}  // namespace nu
