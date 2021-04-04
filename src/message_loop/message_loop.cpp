#include "nucleus/message_loop/message_loop.h"

namespace nu {

MessageLoop::MessageLoop(ScopedPtr<MessagePump> pump) : pump_{std::move(pump)} {
  if (!pump_) {
    pump_ = makeScopedPtr<MessagePumpDefault>();
  }
}

void MessageLoop::post_task(ScopedPtr<Task> task) {
  queue_.emplace_back(std::move(task));

  // Notify the pump that we have tasks to execute.
  pump_->schedule_task();
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

bool MessageLoop::run_task() {
  if (!queue_.empty()) {
    auto task_to_execute = std::move(queue_.front());
    queue_.pop_front();

    task_to_execute->execute();
  }

  if (quit_on_idle_ && queue_.empty()) {
    return false;
  }

  return true;
}

void MessageLoop::run_internal() {
  pump_->run(this);
}

}  // namespace nu
