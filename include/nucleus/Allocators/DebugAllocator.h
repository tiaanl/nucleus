
#ifndef NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_

#include "nucleus/Allocators/Allocator.h"

#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Containers/DynamicArray.h"

namespace nu {

class DebugAllocator : public Allocator {
public:
  explicit DebugAllocator(Allocator* parent = getDefaultAllocator());
  ~DebugAllocator();

  Allocator* getParent() const {
    return m_parent;
  }

  USize getBytesAllocated() const {
    return m_bytesAllocated;
  }
  USize getBytesDeallocated() const {
    return m_bytesAllocated - m_bytesOutstanding;
  }
  USize getBytesOutstanding() const {
    return m_bytesOutstanding;
  }
  USize getMaxAllocated() const {
    return m_maxAllocated;
  }
  USize getBlocksOutstanding() const {
    return m_blocks.getSize();
  }

  static USize getLeakedBytes() {
    return s_leakedBytes;
  }
  static USize getLeakedBlocks() {
    return s_leakedBlocks;
  }
  static void clearLeaked();

protected:
  void* doAllocate(USize bytes, USize alignment) override;
  void doFree(void* p, USize bytes, USize alignment) override;
  bool doIsEqual(const Allocator& other) const noexcept override {
    return this == &other;
  }
  void* doAllocate(USize bytes, USize alignment, const char* file) override;

private:
  struct Record {
    void* ptr;
    USize bytes;
    USize alignment;

    Record(void* ptr, USize bytes, USize alignment) : ptr(ptr), bytes(bytes), alignment(alignment) {}
  };

  Allocator* m_parent;

  USize m_bytesAllocated;
  USize m_bytesOutstanding;
  USize m_maxAllocated;

  nu::DynamicArray<Record> m_blocks;

  static USize s_leakedBytes;
  static USize s_leakedBlocks;
};

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_DEBUG_ALLOCATOR_H_
