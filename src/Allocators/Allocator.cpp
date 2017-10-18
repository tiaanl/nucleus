
#include "nucleus/Allocators/Allocator.h"
#include <cstdlib>

namespace nu {

inline Allocator::Allocator(const char* name) : m_name(name) {}

inline Allocator::Allocator(const Allocator& other) : m_name(other.m_name) {}

inline Allocator::Allocator(const Allocator& other, const char* name) : m_name(name) {}

inline Allocator& Allocator::operator=(const Allocator& other) {
  m_name = other.m_name;

  return *this;
}

inline void Allocator::setName(const char* name) {
  m_name = name;
}

inline void* Allocator::allocate(USize size) {
  return std::malloc(size);
}

inline void* Allocator::allocate(USize size, USize, USize) {
  return std::malloc(size);
}

inline void Allocator::free(void* p, USize size) {
  std::free(p);
}

inline bool operator==(const Allocator& left, const Allocator& right) {
  return true;
}

inline bool operator!=(const Allocator& left, const Allocator& right) {
  return false;
}

}  // namespace nu
