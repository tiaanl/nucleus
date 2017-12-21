
#ifndef NUCLEUS_ATOMICS_ATOMIC_REF_COUNT_H_
#define NUCLEUS_ATOMICS_ATOMIC_REF_COUNT_H_

#include "nucleus/Atomics/Atomics.h"

namespace nu {

typedef detail::Atomic32 AtomicRefCount;

// Increments the reference count by |increment|, which must exceed 0.
inline void atomicRefCountInc(volatile AtomicRefCount* ptr, AtomicRefCount increment) {
  detail::atomicIncrementNoBarrier(ptr, increment);
}

// Decrement the reference count by |decrement|, which much exceed 0, and return whether the result is non-zero.
inline bool atomicRefCountDec(volatile AtomicRefCount* ptr, AtomicRefCount decrement) {
  bool res = (detail::atomicIncrementBarrier(ptr, -decrement) != 0);
  return res;
}

// Increment the reference count by 1.
inline void atomicRefCountInc(volatile AtomicRefCount* ptr) {
  atomicRefCountInc(ptr, 1);
}

// Decrement the reference count by 1 and return whether the result is non-zero.
inline bool atomicRefCountDec(volatile AtomicRefCount* ptr) {
  return atomicRefCountDec(ptr, 1);
}

// Return whether the reference count is one.  If the reference count is used in the conventional way, a reference count
// of 1 implies that the current thread owns the reference and no other threads shares it.
inline bool atomicRefCountIsOne(volatile AtomicRefCount* ptr) {
  bool res = (detail::atomicLoadAcquire(ptr) == 1);
  return res;
}

// Return whether the reference count is zero.  With conventional object reference counting, the object will be
// destroyed, so the reference count should never be zero.  Hence this is generally used for debug check.
inline bool atomicRefCountIsZero(volatile AtomicRefCount* ptr) {
  bool res = (detail::atomicLoadAcquire(ptr) == 0);
  return res;
}

}  // namespace nu

#endif  // NUCLEUS_ATOMICS_ATOMIC_REF_COUNT_H_
