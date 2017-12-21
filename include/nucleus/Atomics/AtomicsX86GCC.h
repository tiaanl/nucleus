
#if !defined(ATOMICS_INTERNAL)
#error "This file should be included as part of Atomics.h"
#endif

inline void atomicMemoryBarrier() {
  __asm__ __volatile__("mfence" : : : "memory");
}

inline Atomic32 atomicCompareAndSwapNoBarrier(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  Atomic32 prev;
  __asm__ __volatile__("lock; cmpxchgl %1,%2" : "=a"(prev) : "q"(newValue), "m"(*ptr), "0"(oldValue) : "memory");
  return prev;
}

inline Atomic32 atomicCompareAndSwapAcquire(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  Atomic32 x = atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
  // if (AtomicOps_Internalx86CPUFeatures.has_amd_lock_mb_bug) {
  //   __asm__ __volatile__("lfence" : : : "memory");
  // }
  return x;
}

inline Atomic32 atomicCompareAndSwapRelease(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  return atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
}

inline Atomic32 atomicExchangeNoBarrier(volatile Atomic32* ptr, Atomic32 newValue) {
  __asm__ __volatile__("xchgl %1,%0" : "=r"(newValue) : "m"(*ptr), "0"(newValue) : "memory");
  return newValue;
}

inline Atomic32 atomicIncrementNoBarrier(volatile Atomic32* ptr, Atomic32 increment) {
  Atomic32 temp = increment;
  __asm__ __volatile__("lock; xaddl %0,%1" : "+r"(temp), "+m"(*ptr) : : "memory");
  return temp + increment;
}

inline Atomic32 atomicIncrementBarrier(volatile Atomic32* ptr, Atomic32 increment) {
  Atomic32 temp = increment;
  __asm__ __volatile__("lock; xaddl %0,%1" : "+r"(temp), "+m"(*ptr) : : "memory");
  // if (AtomicOps_Internalx86CPUFeatures.has_amd_lock_mb_bug) {
  //   __asm__ __volatile__("lfence" : : : "memory");
  // }
  return temp + increment;
}

inline void atomicStoreNoBarrier(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value;
}

inline void atomicStoreAcquire(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value;
  atomicMemoryBarrier();
}

inline void atomicStoreRelease(volatile Atomic32* ptr, Atomic32 value) {
  __asm__ __volatile__("" : : : "memory");
  *ptr = value;
}

inline Atomic32 atomicLoadNoBarrier(volatile const Atomic32* ptr) {
  return *ptr;
}

inline Atomic32 atomicLoadAcquire(volatile const Atomic32* ptr) {
  Atomic32 value = *ptr;
  __asm__ __volatile__("" : : : "memory");
  return value;
}

inline Atomic32 atomicLoadRelease(volatile const Atomic32* ptr) {
  atomicMemoryBarrier();
  return *ptr;
}
