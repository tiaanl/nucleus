#pragma once

#include <deque>

#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/function.h"
#include "nucleus/message_loop/message_pump.h"

namespace nu {

class MessageLoop : public MessagePump::Delegate {
  NU_DELETE_COPY_AND_MOVE(MessageLoop);

public:
  explicit MessageLoop(ScopedPtr<MessagePump> pump = {});

  // Post a task to the queue to be executed.
  void post_task(Function<void()> task);

  // Post a task to the queue, that when execute, will notify the `MessageLoop` to quit when it
  // is idle.
  void post_quit();

  // Continue executing tasks until the queue is empty.
  void run_until_idle();

  // Continue executing tasks until a quit task is posted to the queue.
  void run();

private:
  // Override: MessageLoopDelegate
  bool run_task() override;

  void run_internal();

  ScopedPtr<MessagePump> pump_;
  bool quit_on_idle_ = false;
  std::deque<Function<void()>> queue_;
};

}  // namespace nu
