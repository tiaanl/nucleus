#include "nucleus/HighPerformanceTimer.h"

#include "nucleus/Config.h"
#include "nucleus/Logging.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
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

I64 getHighPerformanceTimerFrequency() {
  LOG(Info) << "Updating high performance timer frequency.";

#if OS(WIN)
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  return frequency.QuadPart;
#elif OS(MACOSX)
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);
  return 1000.0 * rate_nsec.numer / rate_nsec.denom;
#elif OS(POSIX)
  struct timespec specRate;
  clock_getres(CLOCKID, &specRate);
  return 1000.0 * specRate.tv_nsec;
#else
#error Operating system not supported
#endif
}

#if OS(WIN)
struct FrequencyStorage {
  F64 ticksPerSecond;

  FrequencyStorage() noexcept {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    ticksPerSecond = static_cast<F64>(frequency.QuadPart);
  }
};
#endif  // OS(WIN)

FrequencyStorage s_frequencyStorage;

}  // namespace

F64 getCurrentHighPerformanceTick() {
#if OS(WIN)
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return static_cast<F64>(counter.QuadPart) * 1000.0 * 1000.0 / s_frequencyStorage.ticksPerSecond;
#elif OS(MACOSX)
  now = mach_absolute_time();
#elif OS(POSIX)
  struct timespec specTime;
  clock_gettime(CLOCKID, &specTime);
  F64 now = static_cast<F64>(specTime.tv_sec) * 1000000.0 + static_cast<F64>(specTime.tv_nsec);
#else
#error Operating system not supported
#endif
}

}  // namespace nu
