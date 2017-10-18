
#ifndef NUCLEUS_ALLOCATORS_ALLOCATOR_H_
#define NUCLEUS_ALLOCATORS_ALLOCATOR_H_

#include "nucleus/types.h"

namespace nu {

constexpr static USize kMinAlignment = sizeof(void*) * 2;

class Allocator {
public:
  explicit Allocator(const char* name = "Allocator");
  Allocator(const Allocator& other);
  Allocator(const Allocator& other, const char* name);

  Allocator& operator=(const Allocator& other);

  void* allocate(USize size);
  void* allocate(USize size, USize alignment, USize offset);
  void free(void* p, USize size);

  const char* getName() const { return m_name; }
  void setName(const char* name);

protected:
  const char* m_name;
};

bool operator==(const Allocator& left, const Allocator& right);
bool operator!=(const Allocator& left, const Allocator& right);

template <typename Allocator>
inline void* allocateMemory(Allocator& allocator, USize size, USize alignment, USize offset) {
  return (alignment <= kMinAlignment) ? allocator.allocate(size) : allocator.allocate(size, alignment, offset);
}

template <typename Allocator>
inline void freeMemory(Allocator& allocator, void* ptr, USize size) {
  allocator.free(ptr, size);
}

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_ALLOCATOR_H_
