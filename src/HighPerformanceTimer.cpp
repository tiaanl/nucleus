
#include "nucleus/HighPerformanceTimer.h"
#include "nucleus/Config.h"

#if OS(WIN)
#include "nucleus/win/WindowsMixin.h"
#elif OS(MACOSX)
#include <mach/mach_time.h>
#elif OS(POSIX)
#include <time.h>
#ifdef CLOCK_MONOTONIC
#define CLOCKID CLOCK_MONOTONIC
#else
#define CLOCKID CLOCK_REALTIME
#endif
#else
#error Operating system not supported
#endif

namespace nu {

namespace {

F64 g_highPerformanceTimerFrequency = 1.0;

F64 getHighPerformanceTimerFrequency() {
  F64 rate = 0.0;

#if OS(WIN)
  LARGE_INTEGER frequency;
  ::QueryPerformanceFrequency(&frequency);
  rate = static_cast<F64>(frequency.QuadPart);
#elif OS(MACOSX)
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);
  rate = 1000000.0 * rate_nsec.numer / rate_nsec.denom;
#elif OS(POSIX)
  struct timespec specRate;
  clock_getres(CLOCKID, &specRate);
  rate = 1000000.0 * specRate.tv_nsec;
#else
#error Operating system not supported
#endif

  return rate;
}

}  // namespace

F64 getCurrentHighPerformanceTick() {
  F64 rate = g_highPerformanceTimerFrequency;
  if (rate == 1.0) {
    g_highPerformanceTimerFrequency = rate = getHighPerformanceTimerFrequency();
  }

  F64 now = 0.0;

#if OS(WIN)
  LARGE_INTEGER time;
  ::QueryPerformanceCounter(&time);
  now = static_cast<F64>(time.QuadPart) * 1000000.0;
#elif OS(MACOSX)
  now = mach_absolute_time();
#elif OS(POSIX)
  struct timespec specTime;
  clock_gettime(CLOCKID, &specTime);
  now = static_cast<F64>(specTime.tv_sec) * 1000000000.0 + static_cast<F64>(specTime.tv_nsec);
#else
#error Operating system not supported
#endif

  return now / rate;
}

}  // namespace nu
