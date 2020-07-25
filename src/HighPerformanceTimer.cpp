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

struct FrequencyStorage {
  F64 ticksPerSecond;

  FrequencyStorage() noexcept {
#if OS(WIN)
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    ticksPerSecond = static_cast<F64>(frequency.QuadPart);
#elif OS(MACOSX)
    mach_timebase_info_data_t rate;
    mach_timebase_info(&rate);
    ticksPerSecond = static_cast<F64>(rate.numer) * 1000.0 / static_cast<F64>(rate.denom);
#elif OS(POSIX)
    struct timespec rate;
    clock_getres(CLOCKID, &rate);
    ticksPerSecond = static_cast<F64>(rate.tv_nsec) * 1000.0;
#else
#error Operating system not supported
#endif
  }
};

FrequencyStorage s_frequencyStorage;

}  // namespace

F64 getCurrentHighPerformanceTick() {
#if OS(WIN)
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return static_cast<F64>(counter.QuadPart) * 1000.0 * 1000.0 / s_frequencyStorage.ticksPerSecond;
#elif OS(MACOSX)
  auto time = mach_absolute_time();
  return static_cast<F64>(time) / s_frequencyStorage.ticksPerSecond;
#elif OS(POSIX)
  struct timespec specTime;
  clock_gettime(CLOCKID, &specTime);
  F64 now = static_cast<F64>(specTime.tv_sec) * 1000000.0 + static_cast<F64>(specTime.tv_nsec);
#else
#error Operating system not supported
#endif
}

}  // namespace nu
