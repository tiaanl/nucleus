// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <condition_variable>
#include <functional>
#include <queue>
#include <memory>
#include <mutex>
#include <string>

#include "nucleus/macros.h"
#include "nucleus/message_loop/incoming_task_queue.h"
#include "nucleus/message_loop/message_pump.h"
#include "nucleus/message_loop/pending_task.h"

namespace nu {

class MessageLoop : public MessagePump::Delegate {
public:
  using Task = std::function<void()>;

  explicit MessageLoop(std::unique_ptr<MessagePump> pump);
  ~MessageLoop() override;

  // Returns the MessageLoop object for the current thread, or null if none.
  static MessageLoop* getCurrent();

  // Add a task to the queue to be performed.
  void postTask(const Task& task);

  // Add a task that will only be run after a certain delay.
  void postDelayedTask(const Task& task,
                       const std::chrono::milliseconds& delay);

  // Run the message loop.
  void run();

  // Run the message loop until we go idle.
  void runUntilIdle();

  // Requests that the message loop quit as soon as there are no more tasks to
  // perform.
  void quitWhenIdle();

  // Checks whether the message loop is currently running.
  bool isRunning() const;

  // Wakes up the message pump.  Can be called on any thread.  The caller is
  // responsible for synchronizing scheduleWork() calls.
  void scheduleWork();

private:
  std::unique_ptr<MessagePump> m_pump;

  // Do common initialization.
  void initialize();

  // Invokes the actual run loop using the message pump.
  void runHandler();

  // Calls runTask or queues the pendingTask on the deferred task list if it
  // cannot be run right now.  Returns true if the task was run.
  bool deferOrRunPendingTask(const PendingTask& pendingTask);

  // Adds the pending task to m_delayedWorkQueue.
  void addToDelayedWorkQueue(const PendingTask& pendingTask);

  // Run all tasks that haven't been run yet and then clear our the queue.
  // Returns true if any work was performed.
  bool deletePendingTasks();

  // Load tasks from the incoming queue to m_workQueue if the latter is empty.
  void reloadWorkQueue();

  // Override: MessagePump::Delegate
  bool doWork() override;
  bool doDelayedWork(std::chrono::time_point<
      std::chrono::high_resolution_clock>* nextDelayedWorkTime) override;
  bool doIdleWork() override;

  // A list of tasks that need to be processed by this instance.  Note that this
  // queue is only accessed (push/pop) by our current thread.
  std::queue<PendingTask> m_workQueue;

  // Contains delayed tasks, sorted by their delayedRunTime property.
  std::priority_queue<PendingTask> m_delayedWorkQueue;

  // A recent time, used to check m_delayedWorkQueue.
  std::chrono::time_point<std::chrono::high_resolution_clock> m_recentTime;

  // Name of the thread.
  std::string m_threadName;

  // The task queue where we put all the incoming work, before it is scheduled
  // to be run.
  std::unique_ptr<detail::IncomingTaskQueue> m_incomingTaskQueue;

  // This is set to true if the message loop should quit when it runs out of
  // work.
  bool m_quitWhenIdleReceived = false;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_
