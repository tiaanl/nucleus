
#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_

#include <chrono>

namespace nu {

class MessagePump {
public:
  using TimePoint = std::chrono::high_resolution_clock::time_point;

  class Delegate {
  public:
    virtual ~Delegate() = default;

    // Called from within `run` in response to `scheduleWork` or when the message pump would
    // otherwise call `doDelayedWork`.  Returns true to indicate that work was done.
    // `doDelayedWork` will still be called if `doWork` returns true, but `doIdleWork` will not.
    virtual bool doWork() = 0;

    // Called from within `run` just before the message pump goes to sleep.  Returns `true` to
    // indicate that idle work was done.  Returning false means the pump will now wait.
    virtual bool doIdleWork() = 0;
  };

  MessagePump();
  virtual ~MessagePump();

  // The `run` method is called to enter the `MessagePump`'s run loop.
  virtual void run(Delegate* delegate) = 0;

  // Quit immediately from the most recently entered run loop.  This method may only be used on the
  // thread that called Run.
  virtual void quit() = 0;

  // Schedule a `doWork` callback to happen reasonably soon.  Does nothing if a `doWork` callback is
  // already scheduled.  This method may be called from any thread.
  virtual void scheduleWork() = 0;

  // Schedule a `doDelayedWork` callback to happen at the specified time, cancelling any pending
  // `doDelayedWork` callbacks.  This method may only be used on the thread that called `run`.
  // virtual void scheduleDelayedWork(const TimePoint& delayedWorkTime) = 0;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
