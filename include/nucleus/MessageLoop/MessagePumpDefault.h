
#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_

#include <condition_variable>
#include <mutex>

#include "nucleus/Macros.h"
#include "nucleus/MessageLoop/MessagePump.h"

namespace nu {

class MessagePumpDefault : public MessagePump {
public:
  COPY_DELETE(MessagePumpDefault);
  MOVE_DELETE(MessagePumpDefault);

  MessagePumpDefault();
  ~MessagePumpDefault() override;

  // Override: MessagePump
  void run(Delegate* delegate) override;
  void quit() override;
  void scheduleWork() override;
  // void scheduleDelayedWork(const TimePoint& delayedWorkTime) override;

private:
  // This flag is set to `false` when `run` should return.
  bool m_keepRunning = true;

  // The time at which we should call `doDelayedWork`.
  TimePoint m_delayedWorkTime;

  // Used to sleep until there is more work to do.
  std::mutex m_sleep;
  std::condition_variable m_waitForWork;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
