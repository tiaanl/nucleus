
#ifndef NUCLEUS_MEMORY_REF_COUNTED_H_
#define NUCLEUS_MEMORY_REF_COUNTED_H_

#include "nucleus/Atomics/AtomicRefCount.h"
#include "nucleus/Macros.h"

namespace nu {

namespace detail {

class RefCountedBase {
public:
  bool hasOneRef() const;

  void addRef() const;
  bool release() const;

protected:
  RefCountedBase();
  ~RefCountedBase();

  COPY_DELETE(RefCountedBase);
  MOVE_DELETE(RefCountedBase);

private:
  mutable AtomicRefCount m_refCount;
};

template <typename T>
struct DefaultRefCountedTraits {
  static void destruct(const T*) {}
};

}  // namespace detail

template <typename T, typename Traits = detail::DefaultRefCountedTraits<T>>
class RefCounted : public detail::RefCountedBase {
public:
  void addRef() const {
    detail::RefCountedBase::addRef();
  }

  void release() const {
    if (detail::RefCountedBase::release()) {
      Traits::destruct(static_cast<const T*>(this));
    }
  }
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REF_COUNTED_H_
