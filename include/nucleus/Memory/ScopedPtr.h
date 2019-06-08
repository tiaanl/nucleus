
#ifndef NUCLEUS_MEMORY_PTR_H_
#define NUCLEUS_MEMORY_PTR_H_

#include <utility>

#include "nucleus/Macros.h"

namespace nu {

template <typename T>
class ScopedPtr {
public:
  DELETE_COPY(ScopedPtr);

  ScopedPtr() : m_ptr(nullptr) {}

  explicit ScopedPtr(T* other) : m_ptr(other) {}

  ScopedPtr(ScopedPtr&& other) noexcept : m_ptr(other.m_ptr) {
    other.m_ptr = nullptr;
  }

  template <typename U>
  ScopedPtr(ScopedPtr<U>&& other) noexcept : m_ptr(other.release()) {}

  ~ScopedPtr() {
    if (m_ptr) {
      delete m_ptr;
    }
  }

  ScopedPtr& operator=(ScopedPtr&& other) {
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;

    return *this;
  }

  const T* operator->() const {
    return m_ptr;
  }

  T* operator->() {
    return m_ptr;
  }

  const T* get() const {
    return m_ptr;
  }

  T* get() {
    return m_ptr;
  }

  void reset(T* p = nullptr) {
    if (m_ptr) {
      delete m_ptr;
    }

    m_ptr = p;
  }

  T* release() {
    T* ptr = m_ptr;
    m_ptr = nullptr;
    return ptr;
  }

private:
  T* m_ptr;
};

template <typename T, typename... Args>
inline ScopedPtr<T> makeScopedPtr(Args&&... args) {
  return ScopedPtr<T>{new T{std::forward<Args>(args)...}};
}

}  // namespace nu

#endif  // NUCLEUS_MEMORY_PTR_H_
