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

#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_

#include <chrono>

namespace nu {

class MessagePump {
public:
  class Delegate {
  public:
    virtual ~Delegate() {}

    // Called from within run in response to scheduleWork or when the message
    // pump would otherwise call doDelayedWork.  Returns true to indicate that
    // work was done.  doDelayedWork will still be called if doWork returns
    // true, but doIdleWork will not.
    virtual bool doWork() = 0;

    // Called from within run in response to scheduleDelayedWork or when the
    // message pump would otherwise sleep waiting for more work.  Returns true
    // to indicate that delayed work was done.  doIdleWork will not be called if
    // doDelayedWork returns true.  Upon return nextDelayedWorkTime indicates
    // the time when doDelayedWork should be called again.  If
    // nextDelayedWorkTime is null, then the queue of future delayed work (timer
    // events) is currently empty and no additional calls to this function need
    // to be scheduled.
    virtual bool doDelayedWork(std::chrono::time_point<
        std::chrono::high_resolution_clock>* nextDelayedWorkTime) = 0;

    // Called from within run just before the message pump goes to sleep.
    // Returns true to indicate that idle work was done.  Returning false means
    // the pump will not wait.
    virtual bool doIdleWork() = 0;
  };

  MessagePump();
  virtual ~MessagePump();

  // The run method is called to enter the message pump's run loop.
  //
  // Within the method, the message pump is responsible for processing native
  // messages as well as for giving cycles to the delegate periodically.  The
  // message pump should take care to mix delegate callbacks with native message
  // processing so neither type of event starves the other of cycles.
  virtual void run(Delegate* delegate) = 0;

  // Quit immediately from the most recently entered run loop.  This method may
  // only be used on the thread that called run.
  virtual void quit() = 0;

  // Schedule a doWork callback to happen reasonably soon.  Does nothing if a
  // doWork callback is already scheduled.  This method may be called from any
  // thread.  Once this call is made, doWork should not be starved at least
  // until it returns a value of false.
  virtual void scheduleWork() = 0;

  // Schedule a doDelayedWork callback to happen at the specified time,
  // cancelling any pending doDelayedWork callback.  This method may only be
  // used on the thread that called run.
  virtual void scheduleDelayedWork(const std::chrono::time_point<
      std::chrono::high_resolution_clock>& delayedWorkTime) = 0;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_H_
