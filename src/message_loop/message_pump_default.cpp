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

#include "nucleus/message_loop/message_pump_default.h"

#include "nucleus/logging.h"

namespace nu {

MessagePumpDefault::MessagePumpDefault() {
}

MessagePumpDefault::~MessagePumpDefault() {
}

void MessagePumpDefault::run(Delegate* delegate) {
  DCHECK(m_keepRunning) << "Quit must have been called outside of run!";

  for (;;) {
    bool didWork = delegate->doWork();
    if (!m_keepRunning) {
      break;
    }

    didWork |= delegate->doDelayedWork(&m_delayedWorkTime);
    if (!m_keepRunning) {
      break;
    }

    if (didWork) {
      continue;
    }

    didWork = delegate->doIdleWork();
    if (!m_keepRunning) {
      break;
    }

    if (didWork) {
      continue;
    }

    if (m_delayedWorkTime ==
        std::chrono::time_point<std::chrono::high_resolution_clock>()) {
      std::unique_lock<std::mutex> locker(m_lock);
      m_condition.wait(locker);
    } else {
      // std::chrono::milliseconds delay =
      auto delay =
          m_delayedWorkTime - std::chrono::high_resolution_clock::now();
      if (delay > std::chrono::milliseconds()) {
        std::unique_lock<std::mutex> locker(m_lock);
        m_condition.wait_for(locker, delay);
      } else {
        // It looks like m_delayedWorkTime indicates a time in the past, so we
        // need to call doDelayedWork now.
        m_delayedWorkTime =
            std::chrono::time_point<std::chrono::high_resolution_clock>();
      }
    }
  }

  m_keepRunning = true;
}

void MessagePumpDefault::quit() {
  m_keepRunning = false;
}

void MessagePumpDefault::scheduleWork() {
  // Since this can be called on any thread, we need to ensure that our run loop
  // wakes up.
  m_condition.notify_one();
}

void MessagePumpDefault::scheduleDelayedWork(const std::chrono::time_point<
    std::chrono::high_resolution_clock>& delayedWorkTime) {
  m_delayedWorkTime = delayedWorkTime;
}

}  // namespace nu
