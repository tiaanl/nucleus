
#ifndef NUCLEUS_MEMORY_REFCOUNTED_H_
#define NUCLEUS_MEMORY_REFCOUNTED_H_

#include "nucleus/Atomics/AtomicRefCount.h"
#include "nucleus/Macros.h"

namespace nu {

namespace detail {

class RefCountedBase {
public:
  bool hasOneRef() const {
    return m_refCount == 1;
  }

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

template <typename T>
class RefCounted : public detail::RefCountedBase {
public:
  RefCounted() {}

  void addRef() const {
    detail::RefCountedBase::addRef();
  }

  void release() const {
    if (detail::RefCountedBase::release()) {
      delete static_cast<const T*>(this);
    }
  }

protected:
  ~RefCounted() {}

private:
  DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
};

template <typename T, typename Traits>
class RefCountedThreadSafe;

template <typename T>
struct DefaultRefCountedThreadSafeTraits {
  static void destruct(const T* x) {
    RefCountedThreadSafe<T, DefaultRefCountedThreadSafeTraits>::deleteInternal(x);
  }
};

template <typename T, typename Traits = DefaultRefCountedThreadSafeTraits<T>>
class RefCountedThreadSafe : public detail::RefCountedThreadSafeBase {
public:
  RefCountedThreadSafe() {}

  void addRef() const {
    detail::RefCountedThreadSafeBase::addRef();
  }

  void release() const {
    if (detail::RefCountedThreadSafeBase::release()) {
      Traits::destruct(static_cast<const T*>(this));
    }
  }

protected:
  friend struct DefaultRefCountedThreadSafeTraits<T>;

  ~RefCountedThreadSafe() {}

  static void deleteInternal(const T* x) {
    delete x;
  }

  DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafe);
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REFCOUNTED_H_