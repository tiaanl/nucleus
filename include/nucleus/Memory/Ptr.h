
#ifndef NUCLEUS_MEMORY_PTR_H_
#define NUCLEUS_MEMORY_PTR_H_

#include <utility>

#include "nucleus/Macros.h"

namespace nu {

template <typename T>
class Ptr {
public:
  COPY_DELETE(Ptr);

  Ptr() : m_ptr(nullptr) {}

  explicit Ptr(T* other) : m_ptr(other) {}

  Ptr(Ptr&& other) : m_ptr(other.m_ptr) {
    other.m_ptr = nullptr;
  }

  ~Ptr() {
    if (m_ptr) {
      delete m_ptr;
    }
  }

  Ptr& operator=(Ptr&& other) {
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;

    return *this;
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

private:
  T* m_ptr;
};

template <typename T, typename... Args>
inline Ptr<T> makePtr(Args&&... args) {
  return Ptr<T>{new T{std::forward<Args>(args)...}};
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_PTR_H_
