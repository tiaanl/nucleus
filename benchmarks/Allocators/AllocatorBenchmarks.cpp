
#include <nucleus/Allocators/LinearAllocator.h>
#include "benchmark/benchmark.h"
#include "nucleus/Allocators/DebugAllocator.h"

static int iterations = 10;

#if 0
class NoopAllocator : public nu::Allocator {
public:
    void *doAllocate(USize bytes, USize alignment) override {
        return nullptr;
    }

    void doFree(void *p, USize bytes, USize alignment) override {
    }

    bool doIsEqual(const Allocator &other) const noexcept override {
        return true;
    }
};

void benchmarkNoopAllocator(benchmark::State& state) {
  NoopAllocator allocator;

  for (auto _ : state) {
    allocator.allocate(state.range(0));
  }
}

BENCHMARK(benchmarkNoopAllocator)->RangeMultiplier(2)->Range(8, 8<<iterations);
#endif  // 0

void benchmarkGlobalAllocator(benchmark::State &state) {
    nu::GlobalAllocator allocator;

    for (auto _ : state) {
        allocator.allocate(state.range(0));
    }
}

BENCHMARK(benchmarkGlobalAllocator)->RangeMultiplier(2)->Range(8, 8<<iterations);

void benchmarkLinearAllocator(benchmark::State &state) {
    nu::GlobalAllocator parentAllocator;
    nu::LinearAllocator allocator(&parentAllocator, 10 << 10);

    for (auto _ : state) {
        allocator.allocate(state.range(0));
    }
}

BENCHMARK(benchmarkLinearAllocator)->RangeMultiplier(2)->Range(8, 8<<iterations);

#if 0
void benchmarkDebugAllocatorWithGlobalAllocator(benchmark::State &state) {
    nu::DebugAllocator allocator;

    for (auto _ : state) {
        allocator.allocate(state.range(0));
    }
}

BENCHMARK(benchmarkDebugAllocatorWithGlobalAllocator)->RangeMultiplier(2)->Range(8, 8<<iterations);
#endif  // 0

BENCHMARK_MAIN();
