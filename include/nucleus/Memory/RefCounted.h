
#ifndef NUCLEUS_MEMORY_REFCOUNTED_H_
#define NUCLEUS_MEMORY_REFCOUNTED_H_

#include "nucleus/macros.h"
#include "nucleus/Atomics/AtomicRefCount.h"

namespace nu {

namespace detail {

class RefCountedBase {
public:
    bool hasOneRef() const { return m_refCount == 1; }

protected:
    RefCountedBase() : m_refCount(0) {}
    ~RefCountedBase() {}

    void addRef() const {
        ++m_refCount;
    }

    bool release() const {
        return --m_refCount == 0;
    }

private:
    mutable I32 m_refCount;

    DISALLOW_COPY_AND_ASSIGN(RefCountedBase);
};

class RefCountedThreadSafeBase {
public:
    bool hasOneRef() const;

protected:
    RefCountedThreadSafeBase();
    ~RefCountedThreadSafeBase();

    void addRef() const;
    bool release() const;

private:
    mutable AtomicRefCount m_refCount;

    DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REFCOUNTED_H_