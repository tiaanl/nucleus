#pragma once

#include <deque>

#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/message_loop/task.h"

namespace nu {

class MessageLoop {
  NU_DELETE_COPY_AND_MOVE(MessageLoop);

  void post_task(ScopedPtr<Task> task);
  void post_quit();

  // Continue executing tasks until the queue is empty.
  void run_until_idle();

  // Continue executing tasks until a quit task is posted to the queue.
  void run();

public:
  MessageLoop();

private:
  class QuitTask : public Task {
    NU_DELETE_COPY_AND_MOVE(QuitTask);

  public:
    explicit QuitTask(MessageLoop* message_loop) : message_loop_{message_loop} {}

    void execute() override {
      message_loop_->quit_on_idle_ = true;
    }

  private:
    MessageLoop* message_loop_;
  };

  void run_internal();

  bool quit_on_idle_ = false;

  std::deque<ScopedPtr<Task>> queue_;
};

}  // namespace nu
