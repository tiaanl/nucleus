
#include "nucleus/HighPerformanceTimer.h"
#include "nucleus/Config.h"

#if OS(MACOSX)
#include <mach/mach_time.h>
#elif OS(WIN32)
#include "nucleus/win/WindowsMixin.h"
#endif

namespace nu {

F64 getCurrentHighPerformanceTick() {
  F64 rate = 0;
  F64 now = 0;

  // Get the rate.
#if OS(MACOSX)
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);
  rate = 1000000.0 * rate_nsec.numer / rate_nsec.denom;
#elif OS(WIN32)
  LARGE_INTEGER frequency;
  ::QueryPerformanceFrequency(&frequency);
  rate = static_cast<F64>(frequency.QuadPart);
#endif

  // Get the current tick.
#if OS(MACOSX)
  now = mach_absolute_time();
#elif OS(WIN32)
  LARGE_INTEGER time;
  ::QueryPerformanceCounter(&time);
  now = static_cast<F64>(time.QuadPart) * 1000000.0;
#endif

  return now / rate;
}

}  // namespace nu
