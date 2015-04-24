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

#include "nucleus/message_loop/message_loop.h"

#include <cassert>

#include "nucleus/logging.h"

namespace nu {

MessageLoop::MessageLoop(std::unique_ptr<MessagePump> pump) {
  initialize();

  m_pump = std::move(pump);
}

MessageLoop::~MessageLoop() {
  // DCHECK_EQ(this, getCurrent());

  // Clean up any unprocessed tasks, but take care: deleting a task could result
  // in the addition of more tasks.  We set a limit on the number of times we
  // will allow a deleted task to generate more tasks.  Normally, we should only
  // pass through this loop once or twice.  If we end up hitting the loop limit,
  // then it is probably due to one task that is being stubborn.  Inspect the
  // queues to see who is left.
  bool didWork;
  for (int i = 0; i < 100; ++i) {
    deletePendingTasks();
    reloadWorkQueue();
    // If we end up with empty queues, then break out of the loop.
    didWork = deletePendingTasks();
    if (!didWork)
      break;
  }
  // DCHECK(!didWork);

  m_incomingTaskQueue->willDestroyCurrentMessageLoop();
  m_incomingTaskQueue = nullptr;
}

// static
MessageLoop* MessageLoop::getCurrent() {
#if 0
  return lazy_tls_ptr.Pointer()->Get();
#endif  // 0
  return nullptr;
}

void MessageLoop::postTask(const Task& task) {
  postDelayedTask(task, std::chrono::milliseconds(0));
}

void MessageLoop::postDelayedTask(const Task& task,
                                  const std::chrono::milliseconds& delay) {
  m_incomingTaskQueue->addToIncomingQueue(task, delay);
}

void MessageLoop::run() {
  runHandler();
}

void MessageLoop::runUntilIdle() {
  m_quitWhenIdleReceived = true;
  run();
}

void MessageLoop::quitWhenIdle() {
  m_quitWhenIdleReceived = true;
}

bool MessageLoop::isRunning() const {
  //return RunLoop != nullptr;
  return false;
}

void MessageLoop::initialize() {
  // DCHECK(!getCurrent()) << "Should only have one message loop per thread.";

  // TODO: Set the local thread message loop.

  m_incomingTaskQueue = std::make_unique<detail::IncomingTaskQueue>(this);
}

void MessageLoop::runHandler() {
  // DCHECK_EQ(this, getCurrent());
  m_pump->run(this);
}

void MessageLoop::addToDelayedWorkQueue(const PendingTask& pendingTask) {
  // Move to the delayed work queue.
  m_delayedWorkQueue.push(pendingTask);
}

bool MessageLoop::deletePendingTasks() {
  bool didWork = !m_workQueue.empty();
  while (!m_workQueue.empty()) {
    PendingTask pendingTask{m_workQueue.front()};
    m_workQueue.pop();

    if (pendingTask.delayedRunTime !=
        std::chrono::time_point<std::chrono::high_resolution_clock>()) {
      // We want to delete delayed tasks in the same order in which they would
      // normalle be deleted in case of any funny dependencies between delayed
      // tasks.
      addToDelayedWorkQueue(pendingTask);
    }
  }

  while (!m_delayedWorkQueue.empty()) {
    m_delayedWorkQueue.pop();
  }

  return didWork;
}

void MessageLoop::reloadWorkQueue() {
  // We can improve performance of our loading tasks from the incoming queue to
  // workQueue by  waiting until the last minute to load.  That reduces the
  // number of locks-per-task significantly when our queues get large.
  if (m_workQueue.empty()) {
    m_incomingTaskQueue->reloadWorkQueue(&m_workQueue);
  }
}

void MessageLoop::scheduleWork() {
  m_pump->scheduleWork();
}

bool MessageLoop::doWork() {
  for (;;) {
    // Move all tasks from the incoming queue to the m_workQueue. (only if the
    // work queue is empty).
    reloadWorkQueue();

    // If there is nothing to do, then we break out of the loop.
    if (m_workQueue.empty()) {
      break;
    }

    // Start processing all the tasks in the m_workQueue.
    do {
      // Get the top most task.
      PendingTask pendingTask = m_workQueue.front();
      m_workQueue.pop();

      // If the pending task has a valid delayed run time, then we add it to
      // m_delayedWorkQueue.
      if (pendingTask.delayedRunTime !=
          std::chrono::time_point<std::chrono::high_resolution_clock>()) {
        addToDelayedWorkQueue(pendingTask);
        // If we changed the topmost task, then it is time to reschedule.
        // if (m_delayedWorkQueue.top().task == pendingTask.task) {
          m_pump->scheduleDelayedWork(pendingTask.delayedRunTime);
        // }
      } else {
        // The task did not have a delayed run time, so we just run it right
        // now.
        pendingTask.task();

        // We break out here, because we did some work.
        return true;
      }
    } while (!m_workQueue.empty());
  }

  return false;
}

bool MessageLoop::doDelayedWork(std::chrono::time_point<
    std::chrono::high_resolution_clock>* nextDelayedWorkTime) {
  if (m_delayedWorkQueue.empty()) {
    m_recentTime = *nextDelayedWorkTime =
        std::chrono::high_resolution_clock::now();
    return false;
  }

  // When we "fall behind", there wil be a lot of tasks in the delayed work
  // queue that are ready to run.  To increase efficiency when we fall behind,
  // we will only call ::now() intermittently and then process all tasks that
  // are ready to run before calling it again.  As a result, the more we fall
  // behind (and have a lot of ready-to-run delayed tasks), the more efficient
  // we'll be at handling the tasks.

  auto nextRunTime = m_delayedWorkQueue.top().delayedRunTime;
  if (nextRunTime > m_recentTime) {
    // Get a better view of now().
    m_recentTime = std::chrono::high_resolution_clock::now();
    if (nextRunTime > m_recentTime) {
      *nextDelayedWorkTime = nextRunTime;
      return false;
    }
  }

  PendingTask pendingTask = m_delayedWorkQueue.top();
  m_delayedWorkQueue.pop();

  if (!m_delayedWorkQueue.empty()) {
    *nextDelayedWorkTime = m_delayedWorkQueue.top().delayedRunTime;
  }

  //return deferOrRunPendingTask(pendingTask);
  // Run the task.
  pendingTask.task();

  return true;
}

bool MessageLoop::doIdleWork() {
  // processNextDelayedNonNestableTask

  if (m_quitWhenIdleReceived) {
    m_pump->quit();
  }

  return false;
}

}  // namespace nu
