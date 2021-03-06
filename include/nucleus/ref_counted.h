#pragma once

#include <atomic>

#include "nucleus/macros.h"

namespace nu {

namespace detail {

class RefCountedBase {
public:
  NU_DELETE_COPY_AND_MOVE(RefCountedBase);

  bool hasOneRef() const {
    return m_refCount.load(std::memory_order_relaxed) == 1;
  }

  void addRef() const {
    m_refCount.fetch_add(1, std::memory_order_relaxed);
  }

  bool release() const {
    return m_refCount.fetch_sub(1, std::memory_order_release) == 1;
  }

protected:
  RefCountedBase() : m_refCount{0} {};
  ~RefCountedBase() = default;

private:
  mutable std::atomic<MemSize> m_refCount;
};

template <typename T>
struct DefaultRefCountedTraits {
  static void destruct(const T* ptr) {
    delete ptr;
  }
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
