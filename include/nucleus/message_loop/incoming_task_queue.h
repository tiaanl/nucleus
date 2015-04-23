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

#ifndef NUCLEUS_MESSAGE_LOOP_INCOMING_TASK_QUEUE_H_
#define NUCLEUS_MESSAGE_LOOP_INCOMING_TASK_QUEUE_H_

#include <chrono>
#include <functional>
#include <queue>
#include <mutex>

#include "nucleus/macros.h"
#include "nucleus/message_loop/pending_task.h"

namespace nu {

class MessageLoop;

namespace detail {

// Implements a queue of tasks posted to the message loop running on the current
// thread.  This class takes care of synchronizing posting tasks from diffirent
// threads and together with MessageLoop ensures clean shutdown.
class IncomingTaskQueue {
public:
  using Task = std::function<void()>;

  explicit IncomingTaskQueue(MessageLoop* messageLoop);

  // Appends a task to the queue.  Posting of all tasks is routed through
  // addToIncomingQueue() or tryAddToIncomingQueue() to make sure that posting
  // tasks is properly synchronized between threads.  Returns true if the task
  // was successfully added to the queue, otherwise returns false.  In all
  // cases, the ownership of task is transferred to the called method.
  bool addToIncomingQueue(const Task& task,
                          const std::chrono::milliseconds& delay);

  // Loads tasks from the incomingQueue into workQueue.  Must be called from the
  // thread that is running the loop.
  void reloadWorkQueue(std::queue<PendingTask>* workQueue);

  // Disconnects this from the parent message loop.
  void willDestroyCurrentMessageLoop();

private:
  // Calculates the time at which a PendingTask should run.
  std::chrono::time_point<std::chrono::high_resolution_clock>
  calculateDelayedRunTime(const std::chrono::milliseconds& delay);

  // Adds a task to m_incomingQueue.  The caller retains ownership of
  // pendingTask, but this function will reset the value of pendingTask->task.
  // This is needed to ensure that the posting call stack does not retain
  // pendingTask->task beyond this function call.
  bool postPendingTask(PendingTask* pendingTask);

  // The lock that protects access to the members of this class.
  std::mutex m_incomingQueueLock;

  // An incoming queue of tasks that are acquired under a mutex for processing
  // on this instance's tread.  These tasks have not yet been pushed to
  // m_messageLoop.
  std::queue<PendingTask> m_incomingQueue;

  // Points to the message loop that owns this.
  MessageLoop* m_messageLoop;

  // The next sequence number to use for delayed tasks.
  int32_t m_nextSequenceNum = 0;

  // True if our message loop has already been scheduled and does not need to be
  // scheduled again until an empty reload occurs.
  bool m_messageLoopScheduled = false;

  // True if we always need to call scheduleWork when receiving a new task, even
  // if the incoming queue was not empty.
  const bool m_alwaysScheduleWork = false;

  DISALLOW_COPY_AND_ASSIGN(IncomingTaskQueue);
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_INCOMING_TASK_QUEUE_H_
