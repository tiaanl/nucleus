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

#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_

#include <condition_variable>
#include <mutex>

#include "nucleus/macros.h"
#include "nucleus/message_loop/message_pump.h"

namespace nu {

class MessagePumpDefault : public MessagePump {
public:
  MessagePumpDefault();
  ~MessagePumpDefault() override;

  // Override: MessagePump
  void run(Delegate* delegate) override;
  void quit() override;
  void scheduleWork() override;
  void scheduleDelayedWork(const std::chrono::time_point<
      std::chrono::high_resolution_clock>& delayedWorkTime) override;

private:
  // This flag is set to false when run should return.
  bool m_keepRunning = true;

  // Used to sleep until there is more work to do.
  std::mutex m_lock;
  std::condition_variable m_condition;

  // The time at which we should call doDelayedWork.
  std::chrono::time_point<std::chrono::high_resolution_clock> m_delayedWorkTime;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpDefault);
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_PUMP_DEFAULT_H_
