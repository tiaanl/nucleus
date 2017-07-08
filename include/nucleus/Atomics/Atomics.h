
#ifndef NUCLEUS_ATOMICS_ATOMICS_H_
#define NUCLEUS_ATOMICS_ATOMICS_H_

#include "nucleus/config.h"
#include "nucleus/types.h"

namespace nu {

namespace detail {

using Atomic32 = I32;
#if ARCH(CPU_64_BITS)
using Atomic64 = I64;
#endif  // ARCH(CPU_64_BITS)

Atomic32 atomicCompareAndSwap(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue);
Atomic32 atomicExchange(volatile Atomic32* ptr, Atomic32 newValue);
Atomic32 atomicIncrement(volatile Atomic32* ptr, Atomic32 increment);

void memoryBarrier();

void atomicStore(volatile Atomic32* ptr, Atomic32 value);
Atomic32 atomicLoad(volatile const Atomic32* ptr);

#if ARCH(CPU_64_BITS)

Atomic64 atomicCompareAndSwap(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue);
Atomic64 atomicExchange(volatile Atomic64* ptr, Atomic64 newValue);
Atomic64 atomicIncrement(volatile Atomic64* ptr, Atomic64 increment);

void atomicStore(volatile Atomic64* ptr, Atomic64 value);
Atomic64 atomicLoad(volatile const Atomic64* ptr);

#endif  // ARCH(CPU_64_BITS)

#if COMPILER(GCC)

inline Atomic32 atomicCompareAndSwap(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue) {
    Atomic32 prev;
    __asm__ __volatile__("lock; cmpxchgl %1,%2" : "=a"(prev) : "q"(newValue), "m"(*ptr), "0"(oldValue) : "memory");
    return prev;
}

inline Atomic32 atomicExchange(volatile Atomic32* ptr, Atomic32 newValue) {
    __asm__ __volatile__("xchgl %1,%0"  // The lock prefix is implicit for xchg.
                         : "=r"(newValue)
                         : "m"(*ptr), "0"(newValue)
                         : "memory");
    return newValue;
}

inline Atomic32 atomicIncrement(volatile Atomic32* ptr, Atomic32 increment) {
    Atomic32 temp = increment;
    __asm__ __volatile__("lock; xaddl %0,%1" : "+r"(temp), "+m"(*ptr) : : "memory");
    return temp + increment;
}

inline void memoryBarrier() {
    __asm__ __volatile__("mfence" : : : "memory");
}

inline void atomicStore(volatile Atomic32* ptr, Atomic32 value) {
    __asm__ __volatile__("" : : : "memory");
    *ptr = value;
}

inline Atomic32 atomicLoad(volatile const Atomic32* ptr) {
    Atomic32 value = *ptr;
    __asm__ __volatile__("" : : : "memory");
    return value;
}

#if ARCH(CPU_64_BITS)

inline Atomic64 atomicCompareAndSwap(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue) {
    Atomic64 prev;
    __asm__ __volatile__("lock; cmpxchgq %1,%2" : "=a"(prev) : "q"(newValue), "m"(*ptr), "0"(oldValue) : "memory");
    return prev;
}

inline Atomic64 atomicExchange(volatile Atomic64* ptr, Atomic64 newValue) {
    __asm__ __volatile__("xchgq %1,%0" : "=r"(newValue) : "m"(*ptr), "0"(newValue) : "memory");
    return newValue;
}

inline Atomic64 atomicIncrement(volatile Atomic64* ptr, Atomic64 increment) {
    Atomic64 temp = increment;
    __asm__ __volatile__("lock; xaddq %0,%1" : "+r"(temp), "+m"(*ptr) : : "memory");
    return temp + increment;
}

inline void atomicStore(volatile Atomic64* ptr, Atomic64 value) {
    __asm__ __volatile__("" : : : "memory");
    *ptr = value;
}

inline Atomic64 atomicLoad(volatile const Atomic64* ptr) {
    Atomic64 value = *ptr;
    __asm__ __volatile__("" : : : "memory");
    return value;
}

#endif  // ARCH(CPU_64_BITS)

#endif  // COMPILER(GCC)

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_ATOMICS_ATOMICS_H_
