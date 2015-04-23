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

#include "nucleus/message_loop/incoming_task_queue.h"

#include "nucleus/message_loop/message_loop.h"

namespace nu {

namespace detail {

IncomingTaskQueue::IncomingTaskQueue(MessageLoop* messageLoop)
  : m_messageLoop(messageLoop) {
}

bool IncomingTaskQueue::addToIncomingQueue(
    const Task& task, const std::chrono::milliseconds& delay) {
  std::lock_guard<std::mutex> locker(m_incomingQueueLock);
  PendingTask pendingTask{task, calculateDelayedRunTime(delay)};

  return postPendingTask(&pendingTask);
}

void IncomingTaskQueue::reloadWorkQueue(std::queue<PendingTask>* workQueue) {
  // Make sure no tasks are lost.
  // DCHECK(workQueue->empty());

  // Acquire all we can from the inter-thread queue with one lock acquisition.
  std::lock_guard<std::mutex> locker(m_incomingQueueLock);
  if (m_incomingQueue.empty()) {
    // If the loop attempts to reload but there are no tasks in the incoming
    // queue, that means it will go to sleep waiting for more work.  If the
    // incoming queue becomes non-empty we need to schedule it again.
    m_messageLoopScheduled = false;
  } else {
    m_incomingQueue.swap(*workQueue);
  }
}

void IncomingTaskQueue::willDestroyCurrentMessageLoop() {
  std::lock_guard<std::mutex> locker(m_incomingQueueLock);
  m_messageLoop = nullptr;
}

std::chrono::time_point<std::chrono::high_resolution_clock>
IncomingTaskQueue::calculateDelayedRunTime(
    const std::chrono::milliseconds& delay) {
  std::chrono::time_point<std::chrono::high_resolution_clock> delayedRunTime;
  if (delay > std::chrono::milliseconds(0)) {
    delayedRunTime = std::chrono::high_resolution_clock::now() + delay;
  } else {
    // DCHECK_EQ(dealy, 0) << "Delay should not be negative.";
  }
  return delayedRunTime;
}

bool IncomingTaskQueue::postPendingTask(PendingTask* pendingTask) {
  if (!m_messageLoop) {
    pendingTask->task = nullptr;
    return false;
  }

  pendingTask->sequenceNum = m_nextSequenceNum++;

  bool wasEmpty = m_incomingQueue.empty();
  m_incomingQueue.push(*pendingTask);
  pendingTask->task = nullptr;

  if (m_alwaysScheduleWork || (!m_messageLoopScheduled && wasEmpty)) {
    // Wake up the message loop.
    m_messageLoop->scheduleWork();

    // After we've scheduled the message loop, we do not need to do so again
    // until we know it has processed all of the work in our queue and is
    // waiting for more work again.  The message loop will always attempt to
    // reload from the incoming queue before waiting again so we clear this flag
    // in reloadWorkQueue().
    m_messageLoopScheduled = true;
  }

  return true;
}

}  // namespace detail

}  // namespace nu
