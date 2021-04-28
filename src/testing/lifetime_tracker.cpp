#include "nucleus/testing/lifetime_tracker.h"

namespace nu::testing {

I32 LifetimeTracker::creates = 0;
I32 LifetimeTracker::destroys = 0;
I32 LifetimeTracker::copies = 0;
I32 LifetimeTracker::moves = 0;

}  // namespace nu::testing
