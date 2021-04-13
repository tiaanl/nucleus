#include "nucleus/message_loop/message_loop.h"

namespace nu {

MessageLoop::MessageLoop(ScopedPtr<MessagePump> pump) : pump_{std::move(pump)} {
  if (!pump_) {
    pump_ = makeScopedPtr<MessagePumpDefault>();
  }
}

void MessageLoop::post_task(Function<void()> function) {
  std::lock_guard<std::mutex> locker(incoming_tasks_lock_);
  incoming_tasks_.emplace_back(std::move(function));

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

bool MessageLoop::progress() {
  std::vector<Function<void()>> tasks_to_run;

  {
    std::lock_guard<std::mutex> locker{incoming_tasks_lock_};
    incoming_tasks_.swap(tasks_to_run);
  }

  // If we fetched the tasks from the incoming queue and it's empty, then we are idle.
  if (quit_on_idle_ && tasks_to_run.empty()) {
    return false;
  }

  for (const auto& task : tasks_to_run) {
    task();
  }

  return true;
}

void MessageLoop::run_internal() {
  pump_->run(this);
}

}  // namespace nu
