
#include "nucleus/synchronization/lock.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

Lock::Lock() = default;

Lock::~Lock() = default;

void Lock::Acquire() {}

void Lock::Release() {}

}  // namespace nu
