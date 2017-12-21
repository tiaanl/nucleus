
#ifndef NUCLEUS_ALLOCATORS_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_ALLOCATOR_H_

#include "nucleus/Memory/Allocated.h"
#include "nucleus/Types.h"
#include "nucleus/Utils/Move.h"

#undef free

namespace nu {

class Allocator {
public:
  virtual ~Allocator() = default;

  void* allocate(USize bytes, USize alignment = kMaxAlign) {
    return doAllocate(bytes, alignment);
  }

  void free(void* p, USize bytes, USize alignment = kMaxAlign) {
    return doFree(p, bytes, alignment);
  }

  bool isEqual(const Allocator& other) const noexcept {
    return doIsEqual(other);
  }

  template <typename T, typename... Args>
  Allocated<T> construct(Args&&... args) {
    USize size = sizeof(T);
    USize alignment = alignof(T);
    void* data = doAllocate(size, alignment);
    return Allocated<T>{this, new (data) T(forward<Args>(args)...), size, alignment};
  }

protected:
  virtual void* doAllocate(USize bytes, USize alignment) = 0;
  virtual void doFree(void* p, USize bytes, USize alignment) = 0;
  virtual bool doIsEqual(const Allocator& other) const = 0;

private:
  static constexpr USize kMaxAlign = alignof(MaxAlign);
};

inline bool operator==(const Allocator& left, const Allocator& right) {
  return (&left == &right) || left.isEqual(right);
}

inline bool operator!=(const Allocator& left, const Allocator& right) {
  return !(left == right);
}

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_ALLOCATOR_H_
