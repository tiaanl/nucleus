
#ifndef NUCLEUS_MEMORY_PTR_H_
#define NUCLEUS_MEMORY_PTR_H_

#include <utility>

#include "nucleus/Macros.h"

namespace nu {

template <typename T>
class ScopedPtr {
public:
  NU_DELETE_COPY(ScopedPtr);

  constexpr ScopedPtr() noexcept : m_ptr{nullptr} {}

  constexpr ScopedPtr(T* other) : m_ptr{other} {}

  ScopedPtr(ScopedPtr&& other) noexcept : m_ptr{other.m_ptr} {
    other.m_ptr = nullptr;
  }

  template <typename U>
  ScopedPtr(ScopedPtr<U>&& other) noexcept : m_ptr{other.release()} {}

  ~ScopedPtr() {
    // No need to check for `nullptr` before `delete`.
    delete m_ptr;
  }

  ScopedPtr& operator=(ScopedPtr&& other) noexcept {
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;

    return *this;
  }

  explicit operator bool() const {
    return !is_null();
  }

  bool operator==(const ScopedPtr& other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator==(T* other) const {
    return m_ptr == other;
  }

  bool operator!=(const ScopedPtr& other) const {
    return m_ptr != other.m_ptr;
  }

  bool operator!=(const T* other) const {
    return m_ptr != other;
  }

  const T* operator->() const {
    return m_ptr;
  }

  T* operator->() {
    return m_ptr;
  }

  const T& operator*() const {
    return *m_ptr;
  }

  bool is_null() const {
    return m_ptr == nullptr;
  }

  const T* get() const {
    return m_ptr;
  }

  T* get() {
    return m_ptr;
  }

  template <typename U>
  const U* cast() const {
    return static_cast<const U*>(m_ptr);
  }

  template <typename U>
  U* cast() {
    return static_cast<U*>(m_ptr);
  }

  void reset(T* p = nullptr) {
    // No need to check for `nullptr` before `delete`.
    delete m_ptr;
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
