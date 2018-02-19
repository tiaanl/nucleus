
#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_

namespace nu {

class MessagePump {
public:
  class Delegate {
  public:
    virtual ~Delegate();

    // Called from within `run` to `scheduleWork`.  Returns `true` to indicate that work was done.  `doIdleWork` will
    // not be called if `doWork` returns true.
    virtual bool doWork() = 0;

    // Called from within `run` just before the message pump goes to sleep.  Returns true to indicate that idle work was
    // done.
    virtual bool doIdleWork() = 0;
  };

  MessagePump();
  virtual ~MessagePump();

  // The `run` function is called to enter the message pump's run loop.
  virtual void run(Delegate* delegate) = 0;

  // Quit immediately from the most recent entered run loop.  This method may only be used on the thread that called
  // `run`.
  virtual void quit() = 0;

  // Schedule a `doWork` callback to happen reasonably soon.  Does nothing if a `doWork` callback is already scheduled.
  // This function may be called from any thread.
  virtual void scheduleWork() = 0;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
