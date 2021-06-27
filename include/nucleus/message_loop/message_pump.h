#pragma once

#include "nucleus/macros.h"

namespace nu {

class MessagePump {
public:
  class Delegate {
  public:
    virtual ~Delegate() = default;

    // Returns true if the pump should continue working.
    virtual bool progress() = 0;
  };

  virtual ~MessagePump() = default;

  virtual void schedule_task() = 0;
  virtual void run(Delegate* delegate) = 0;
};

class MessagePumpDefault : public MessagePump {
  NU_DELETE_COPY_AND_MOVE(MessagePumpDefault);

public:
  MessagePumpDefault() = default;
  ~MessagePumpDefault() override = default;

  // Override: MessagePump
  void schedule_task() override;
  void run(Delegate* delegate) override;
};

}  // namespace nu
