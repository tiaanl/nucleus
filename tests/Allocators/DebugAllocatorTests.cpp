
#include "nucleus/Allocators/DebugAllocator.h"
#include "nucleus/Testing.h"

void expectDebugValues(const nu::DebugAllocator& debug, USize bytesAllocated, USize bytesFreed, USize bytesOutstanding,
                       USize maxAllocated, USize blocksOutstanding) {
  CHECK(debug.getBytesAllocated() == bytesAllocated);
  CHECK(debug.getBytesFreed() == bytesFreed);
  CHECK(debug.getBytesOutstanding() == bytesOutstanding);
  CHECK(debug.getMaxAllocated() == maxAllocated);
  CHECK(debug.getBlocksOutstanding() == blocksOutstanding);
}

TEST_CASE("allocate from debug allocator") {
  nu::GlobalAllocator global;
  nu::DebugAllocator debug{&global};

  void* p1 = debug.allocate(16, 16);
  expectDebugValues(debug, 16, 0, 16, 16, 1);

  debug.free(p1, 16, 16);
  expectDebugValues(debug, 16, 16, 0, 16, 0);
}
