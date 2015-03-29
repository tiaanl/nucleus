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

#include "base/message_loop/message_loop.h"

#include <cassert>

#include "base/logging.h"

namespace base {

MessageLoop::MessageLoop() {
}

MessageLoop::~MessageLoop() {
}

void MessageLoop::postTask(const Task& callback) {
  m_incomingQueue.push(callback);
}

void MessageLoop::run() {
  assert(!m_shouldQuit);

  m_shouldQuit = false;
  for (;;) {
    LOG(Info) << "Trying to do work.";

    bool didWork = doWork();

    if (m_shouldQuit)
      break;

    if (didWork)
      continue;

    didWork |= doIdleWork();

    if (m_shouldQuit)
      break;
  }

  assert(m_shouldQuit);
}

void MessageLoop::runUntilIdle() {
  m_quitWhenIdle = true;
  run();
}

void MessageLoop::requestQuit() {
  postTask(std::bind(&MessageLoop::quitInternal, this));
}

bool MessageLoop::doWork() {
  for (;;) {
    reloadWorkQueue();

    if (m_workQueue.empty())
      break;

    // Start executing tasks.
    do {
      Task callback = m_workQueue.front();
      m_workQueue.pop();

      callback();
    } while (!m_workQueue.empty());

    return true;
  }

  // Nothing happened.
  return false;
}

bool MessageLoop::doIdleWork() {
  if (m_quitWhenIdle) {
    m_quitWhenIdle = false;
    m_shouldQuit = true;
    return true;
  }

  return false;
}

void MessageLoop::reloadWorkQueue() {
  // Make sure the queue is empty before we put new tasks in there.
  assert(m_workQueue.empty());

  // TODO(tiaanl): Lock access to the incoming queue.
  if (!m_incomingQueue.empty())
    m_incomingQueue.swap(m_workQueue);

  // All the work should be in the work queue now.
  assert(m_incomingQueue.empty());
}

void MessageLoop::quitInternal() {
  m_shouldQuit = true;
}

}  // namespace base
