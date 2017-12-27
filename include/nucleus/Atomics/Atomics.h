
#ifndef NUCLEUS_ATOMICS_ATOMICS_H_
#define NUCLEUS_ATOMICS_ATOMICS_H_

#include "nucleus/Config.h"
#include "nucleus/Types.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#endif  // OS(WIN)

namespace nu {

namespace detail {

using Atomic32 = I32;

#if ARCH(CPU_64_BITS)
using Atomic64 = I64;
#endif

#if ARCH(CPU_64_BITS)
using AtomicWord = Atomic64;
#elif ARCH(CPU_32_BITS)
using AtomicWord = Atomic32;
#endif

void atomicMemoryBarrier();

// 32-bits

Atomic32 atomicCompareAndSwapNoBarrier(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue);
Atomic32 atomicCompareAndSwapAcquire(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue);
Atomic32 atomicCompareAndSwapReleae(volatile Atomic32* ptr, Atomic32 oldValue, Atomic32 newValue);

Atomic32 atomicExchangeNoBarrier(volatile Atomic32* ptr, Atomic32 newValue);

Atomic32 atomicIncrementNoBarrier(volatile Atomic32* ptr, Atomic32 increment);
Atomic32 atomicIncrementBarrier(volatile Atomic32* ptr, Atomic32 increment);

void atomicStoreNoBarrier(volatile Atomic32* ptr, Atomic32 value);
void atomicStoreAcquire(volatile Atomic32* ptr, Atomic32 value);
void atomicStoreRelease(volatile Atomic32* ptr, Atomic32 value);

Atomic32 atomicLoadNoBarrier(volatile const Atomic32* ptr);
Atomic32 atomicLoadAcquire(volatile const Atomic32* ptr);
Atomic32 atomicLoadRelease(volatile const Atomic32* ptr);

// 64-bits

#if ARCH(CPU_64_BITS)

Atomic64 atomicCompareAndSwapNoBarrier(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue);
Atomic64 atomicCompareAndSwapAcquire(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue);
Atomic64 atomicCompareAndSwapReleae(volatile Atomic64* ptr, Atomic64 oldValue, Atomic64 newValue);

Atomic64 atomicExchangeNoBarrier(volatile Atomic64* ptr, Atomic64 newValue);

Atomic64 atomicIncrementNoBarrier(volatile Atomic64* ptr, Atomic64 increment);
Atomic64 atomicIncrementBarrier(volatile Atomic64* ptr, Atomic64 increment);

void atomicStoreNoBarrier(volatile Atomic64* ptr, Atomic64 value);
void atomicStoreAcquire(volatile Atomic64* ptr, Atomic64 value);
void atomicStoreRelease(volatile Atomic64* ptr, Atomic64 value);

Atomic64 atomicLoadNoBarrier(volatile const Atomic64* ptr);
Atomic64 atomicLoadAcquire(volatile const Atomic64* ptr);
Atomic64 atomicLoadRelease(volatile const Atomic64* ptr);

#endif  // OS(CPU_64_BITS)

#define ATOMICS_INTERNAL

#if COMPILER(GCC)
#include "nucleus/Atomics/AtomicsX86GCC.h"
#endif  // COMPILER(GCC)

#if COMPILER(MSVC)
#include "nucleus/Atomics/AtomicsX86MSVC.h"
#endif  // COMPILER(MSVC)

#undef ATOMICS_INTERNAL

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_ATOMICS_ATOMICS_H_
