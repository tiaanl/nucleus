#include "nucleus/message_loop/message_pump.h"

namespace nu {

void MessagePumpDefault::schedule_task() {}

void MessagePumpDefault::run(Delegate* delegate) {
  for (;;) {
    bool has_more_work = delegate->progress();

    if (!has_more_work) {
      break;
    }
  }
}

}  // namespace nu
