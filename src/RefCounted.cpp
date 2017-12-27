
#include "nucleus/RefCounted.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

bool RefCounted::hasOneRef() const {
  return atomicRefCountIsOne(&const_cast<RefCounted*>(this)->m_refCount);
}

void RefCounted::addRef() const {
  atomicRefCountInc(&m_refCount);
}

bool RefCounted::release() const {
  return !atomicRefCountDec(&m_refCount);
}

RefCounted::RefCounted() : m_refCount(0) {}

RefCounted::~RefCounted() = default;

}  // namespace nu
