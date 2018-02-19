
#include "nucleus/MessageLoop/MessagePumpDefault.h"
#include "nucleus/Logging.h"

namespace nu {

MessagePumpDefault::MessagePumpDefault() = default;

MessagePumpDefault::~MessagePumpDefault() = default;

void MessagePumpDefault::run(Delegate* delegate) {
  DCHECK(m_keepRunning) << "Quit must have been called outside of run!";

  for (;;) {
    bool didWork = delegate->doWork();
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

    auto lock = std::unique_lock<std::mutex>{m_sleep};
    m_waitForWork.wait(lock);
  }
}

void MessagePumpDefault::quit() {
  m_keepRunning = false;
}

void MessagePumpDefault::scheduleWork() {
  // Since this can be called on any thread, we need to ensure that our `run` loop wakes up.
  m_waitForWork.notify_one();
}

}  // namespace nu
