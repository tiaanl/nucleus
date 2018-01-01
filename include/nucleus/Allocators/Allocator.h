
#ifndef NUCLEUS_ALLOCATORS_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_ALLOCATOR_H_

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

template <typename T, typename... Args>
T* construct(Allocator* allocator, Args&&... args) {
  void* data = allocator->allocate(sizeof(T), alignof(T));
  return new (data) T(forward(args)...);
}

template <typename T>
void destruct(Allocator* allocator, T* ptr) {
  ptr->~T();
  allocator->free(ptr, sizeof(T), alignof(T));
}

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_ALLOCATOR_H_
