#include "nucleus/message_loop/message_pump.h"

namespace nu {

void MessagePumpDefault::schedule_task() {}

void MessagePumpDefault::run(Delegate* delegate) {
  for (;;) {
    bool keep_running = delegate->progress();
    if (!keep_running) {
      break;
    }
  }
}

}  // namespace nu
