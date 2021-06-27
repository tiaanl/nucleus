#pragma once

#include "nucleus/macros.h"
#include "nucleus/types.h"

namespace nu {

// Return a time since an unspecified time in microseconds.
F64 getTimeInMicroseconds();

class Timer {
public:
  Timer() : startTime_{getTimeInMicroseconds()} {}

  // Return the time elapsed since the timer was started in microseconds.
  NU_NO_DISCARD F64 elapsed() const {
    return getTimeInMicroseconds() - startTime_;
  }

private:
  F64 startTime_;
};

}  // namespace nu
