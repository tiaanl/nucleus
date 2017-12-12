
#ifndef NUCLEUS_ALLOCATORS_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_ALLOCATOR_H_

#include <atomic>

#include "nucleus/Atomics/Atomics.h"
#include "nucleus/types.h"

#ifdef _CRTDBG_MAP_ALLOC
#undef free
#endif

namespace nu {

class Allocator {
public:
  virtual ~Allocator() {}

  void* allocate(USize bytes, USize alignment = kMaxAlign) {
    return doAllocate(bytes, alignment);
  }
  void free(void* p, USize bytes, USize alignment = kMaxAlign) {
    return doFree(p, bytes, alignment);
  }
  bool isEqual(const Allocator& other) const noexcept {
    return doIsEqual(other);
  }

protected:
  virtual void* doAllocate(USize bytes, USize alignment) = 0;
  virtual void doFree(void* p, USize bytes, USize alignment) = 0;
  virtual bool doIsEqual(const Allocator& other) const noexcept = 0;

private:
  static constexpr USize kMaxAlign = alignof(max_align_t);
};

inline bool operator==(const Allocator& left, const Allocator& right) {
  return (&left == &right) || left.isEqual(right);
}

inline bool operator!=(const Allocator& left, const Allocator& right) {
  return !(left == right);
}

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_ALLOCATOR_H_
