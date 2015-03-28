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

#ifndef BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "base/macros.h"

namespace base {

class MessageLoop {
public:
  using CallbackType = std::function<void()>;

  MessageLoop();
  ~MessageLoop();

  // Add a task to the queue to be performed.
  void addTask(const std::function<void()>& callback);

  // Run the message loop.
  void run();

  // Request that the message loop quit as soon as possible.
  void requestQuit();

private:
  // Move work from the incoming work queue to the work queue.
  void reloadWorkQueue();

  // Perform any immediate tasks in the work queue.
  bool doWork();

  // Post a task to the queue that will set the loop to exit as soon as
  // possible.
  void quitInternal();

  // Track whether the message loop is running or now.
  bool m_isRunning{false};

  // The queue where incoming tasks will be added.
  std::queue<CallbackType> m_incomingQueue;

  // The queue of tasks that we need to perform.
  std::queue<CallbackType> m_workQueue;

  // The condition that is notified when we have work to do.
  std::condition_variable m_workIsAvailable;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace base

#endif  // BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_
