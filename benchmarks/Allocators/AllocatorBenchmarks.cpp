
#include <nucleus/Allocators/LinearAllocator.h>
#include "benchmark/benchmark.h"
#include "nucleus/Allocators/DebugAllocator.h"

static constexpr int iterations = 1;

static void benchmarkAllocator(benchmark::State& s, nu::Allocator* allocator) {
  for (auto _ : s) {
    const USize bytesToAllocate = sizeof(U64) * s.range(0);

    U64* p = static_cast<U64*>(allocator->allocate(bytesToAllocate));
    assert(p != nullptr);
    for (I32 i = 0; i < s.range(0); ++i) {
      p[i] = p[i] % 255;
    }
    allocator->free(p, s.range(0));
  }
}

#if 0
void benchmarkGlobalAllocator(benchmark::State& state) {
  nu::GlobalAllocator allocator;
  benchmarkAllocator(state, &allocator);
}

BENCHMARK(benchmarkGlobalAllocator)->RangeMultiplier(2)->Range(8, 8 << iterations);
#endif  // 0

void benchmarkLinearAllocator(benchmark::State& state) {
  nu::GlobalAllocator parentAllocator;
  USize bufferSize = state.range(0) * sizeof(U64);
  void* buffer = parentAllocator.allocate(bufferSize);
  nu::LinearAllocator allocator(buffer, bufferSize);
  benchmarkAllocator(state, &allocator);
  parentAllocator.free(buffer, bufferSize);
}

BENCHMARK(benchmarkLinearAllocator)->RangeMultiplier(2)->Range(8, 8 << iterations);

#if 0
void benchmarkDebugAllocatorWithGlobalAllocator(benchmark::State& state) {
  nu::DebugAllocator allocator;
  benchmarkAllocator(state, &allocator);
}

BENCHMARK(benchmarkDebugAllocatorWithGlobalAllocator)->RangeMultiplier(2)->Range(8, 8 << iterations);
#endif  // 0

BENCHMARK_MAIN();
