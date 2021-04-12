#include "nucleus/message_loop/message_loop.h"

namespace nu {

MessageLoop::MessageLoop(ScopedPtr<MessagePump> pump) : pump_{std::move(pump)} {
  if (!pump_) {
    pump_ = makeScopedPtr<MessagePumpDefault>();
  }
}

void MessageLoop::post_task(Function<void()> function) {
  queue_.emplace_back(std::move(function));

  // Notify the pump that we have tasks to execute.
  pump_->schedule_task();
}

void MessageLoop::post_quit() {
  post_task([this]() {
    quit_on_idle_ = true;
  });
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

    task_to_execute();
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
