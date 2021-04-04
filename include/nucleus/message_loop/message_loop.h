#pragma once

#include <deque>

#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/message_loop/message_pump.h"
#include "nucleus/task.h"

namespace nu {

class MessageLoop : public MessagePump::Delegate {
  NU_DELETE_COPY_AND_MOVE(MessageLoop);

public:
  explicit MessageLoop(ScopedPtr<MessagePump> pump = {});

  // Post a task to the queue to be executed.
  void post_task(ScopedPtr<Task> task);

  // Post a task to the queue, that when execute, will notify the `MessageLoop` to quit when it is
  // idle.
  void post_quit();

  // Continue executing tasks until the queue is empty.
  void run_until_idle();

  // Continue executing tasks until a quit task is posted to the queue.
  void run();

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

  // Override: MessageLoopDelegate
  bool run_task() override;

  void run_internal();

  ScopedPtr<MessagePump> pump_;
  bool quit_on_idle_ = false;
  std::deque<ScopedPtr<Task>> queue_;
};

}  // namespace nu
