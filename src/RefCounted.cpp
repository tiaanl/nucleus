
#include "nucleus/RefCounted.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace detail {

bool RefCountedBase::hasOneRef() const {
  return atomicRefCountIsOne(&const_cast<RefCountedBase*>(this)->m_refCount);
}

void RefCountedBase::addRef() const {
  atomicRefCountInc(&m_refCount);
}

bool RefCountedBase::release() const {
  return !atomicRefCountDec(&m_refCount);
}

RefCountedBase::RefCountedBase() : m_refCount(0) {}

RefCountedBase::~RefCountedBase() = default;

}  // namespace detail

}  // namespace nu
