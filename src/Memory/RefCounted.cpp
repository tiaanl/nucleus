
#include "nucleus/Memory/RefCounted.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

namespace detail {

bool RefCountedThreadSafeBase::hasOneRef() const {
  return atomicRefCountIsOne(&const_cast<RefCountedThreadSafeBase*>(this)->m_refCount);
}

RefCountedThreadSafeBase::RefCountedThreadSafeBase() : m_refCount(0) {}

RefCountedThreadSafeBase::~RefCountedThreadSafeBase() = default;

void RefCountedThreadSafeBase::addRef() const {
  atomicRefCountInc(&m_refCount);
}

bool RefCountedThreadSafeBase::release() const {
  return !atomicRefCountDec(&m_refCount);
}

}  // namespace detail

}  // namespace nu
