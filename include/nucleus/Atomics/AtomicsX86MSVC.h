
#if !defined(ATOMICS_INTERNAL)
#error "This file should be included as part of Atomics.h"
#endif

#if ARCH(CPU_64_BITS)
#undef MemoryBarrier
#endif  // ARCH(CPU_64_BITS)

inline void atomicMemoryBarrier() {
#if ARCH(CPU_64_BITS)
  __faststorefence();
#else
  ::MemoryBarrier();
#endif
}

inline Atomic32 atomicCompareAndSwapNoBarrier(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  LONG result = _InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(newValue),
                                            static_cast<LONG>(oldValue));
  return static_cast<Atomic32>(result);
}

inline Atomic32 atomicCompareAndSwapAcquire(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  return atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
}

inline Atomic32 atomicCompareAndSwapRelease(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
  return atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
}

inline Atomic32 atomicExchangeNoBarrier(volatile Atomic32* ptr, Atomic32 newValue) {
  LONG result = _InterlockedExchange(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(newValue));
  return static_cast<Atomic32>(result);
}

inline Atomic32 atomicIncrementBarrier(volatile Atomic32* ptr, Atomic32 increment) {
  return _InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(ptr), static_cast<LONG>(increment)) + increment;
}

inline Atomic32 atomicIncrementNoBarrier(volatile Atomic32* ptr, Atomic32 increment) {
  return atomicIncrementBarrier(ptr, increment);
}

inline void atomicStoreNoBarrier(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value;
}

inline void atomicStoreAcquire(volatile Atomic32* ptr, Atomic32 value) {
  atomicExchangeNoBarrier(ptr, value);
  // acts as a barrier in this implementation
}

inline void atomicStoreRelease(volatile Atomic32* ptr, Atomic32 value) {
  *ptr = value;
}

inline Atomic32 atomicLoadNoBarrier(volatile const Atomic32* ptr) {
  return *ptr;
}

inline Atomic32 atomicLoadAcquire(volatile const Atomic32* ptr) {
  Atomic32 value = *ptr;
  return value;
}

inline Atomic32 atomicLoadRelease(volatile const Atomic32* ptr) {
  atomicMemoryBarrier();
  return *ptr;
}

#if ARCH(CPU_64_BITS)

static_assert(sizeof(Atomic64) == sizeof(PVOID), "atomic word is atomic");

inline Atomic64 atomicCompareAndSwapNoBarrier(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue) {
  PVOID result = InterlockedCompareExchangePointer(
      reinterpret_cast<volatile PVOID*>(ptr), reinterpret_cast<PVOID>(newValue), reinterpret_cast<PVOID>(oldValue));
  return reinterpret_cast<Atomic64>(result);
}

inline Atomic64 atomicCompareAndSwapAcquire(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue) {
  return atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
}

inline Atomic64 atomicCompareAndSwapRelease(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue) {
  return atomicCompareAndSwapNoBarrier(ptr, oldValue, newValue);
}

inline Atomic64 atomicExchangeNoBarrier(volatile Atomic64* ptr, Atomic64 newValue) {
  PVOID result = InterlockedExchangePointer(reinterpret_cast<volatile PVOID*>(ptr), reinterpret_cast<PVOID>(newValue));
  return reinterpret_cast<Atomic64>(result);
}

inline Atomic64 atomicIncrementBarrier(volatile Atomic64* ptr, Atomic64 increment) {
  return InterlockedExchangeAdd64(reinterpret_cast<volatile LONGLONG*>(ptr), static_cast<LONGLONG>(increment)) +
         increment;
}

inline Atomic64 atomicIncrementNoBarrier(volatile Atomic64* ptr, Atomic64 increment) {
  return atomicIncrementBarrier(ptr, increment);
}

inline void atomicStoreNoBarrier(volatile Atomic64* ptr, Atomic64 value) {
  *ptr = value;
}

inline void atomicStoreAcquire(volatile Atomic64* ptr, Atomic64 value) {
  atomicExchangeNoBarrier(ptr, value);
  // acts as a barrier in this implementation
}

inline void atomicStoreRelease(volatile Atomic64* ptr, Atomic64 value) {
  *ptr = value;
}

inline Atomic64 atomicLoadNoBarrier(volatile const Atomic64* ptr) {
  return *ptr;
}

inline Atomic64 atomicLoadAcquire(volatile const Atomic64* ptr) {
  Atomic64 value = *ptr;
  return value;
}

inline Atomic64 atomicLoadRelease(volatile const Atomic64* ptr) {
  atomicMemoryBarrier();
  return *ptr;
}

#endif  // ARCH(CPU_64_BITS)
