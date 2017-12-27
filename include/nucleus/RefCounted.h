
#ifndef NUCLEUS_MEMORY_REFCOUNTED_H_
#define NUCLEUS_MEMORY_REFCOUNTED_H_

#include "nucleus/Atomics/AtomicRefCount.h"
#include "nucleus/Macros.h"

namespace nu {

class RefCounted {
public:
  bool hasOneRef() const;

  void addRef() const;
  bool release() const;

protected:
  RefCounted();
  ~RefCounted();

  COPY_DELETE(RefCounted);
  MOVE_DELETE(RefCounted);

private:
  mutable AtomicRefCount m_refCount;
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REFCOUNTED_H_
