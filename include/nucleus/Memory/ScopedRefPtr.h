
#ifndef NUCLEUS_MEMORY_REF_H_
#define NUCLEUS_MEMORY_REF_H_

namespace nu {

template <typename T>
class ScopedRefPtr {
public:
  ScopedRefPtr() : m_ptr(nullptr) {}

  ScopedRefPtr(T* ptr) : m_ptr(ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  ScopedRefPtr(const ScopedRefPtr<T>& other) : m_ptr(other.m_ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  template <typename U>
  ScopedRefPtr(const ScopedRefPtr<U>& other) : m_ptr(other.get()) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  ~ScopedRefPtr() {
    if (m_ptr) {
      m_ptr->release();
    }
  }

  bool isEmpty() const {
    return !m_ptr;
  }

  T* get() const {
    return m_ptr;
  }

  operator T*() const {
    return m_ptr;
  }

  T* operator->() const {
    return m_ptr;
  }

  ScopedRefPtr<T>& operator=(T* ptr) {
    // addRef() first so that self assignment works.
    if (ptr) {
      ptr->addRef();
    }

    T* old = m_ptr;
    m_ptr = ptr;

    if (old) {
      old->release();
    }

    return *this;
  }

  ScopedRefPtr<T>& operator=(const ScopedRefPtr<T>& other) {
    return *this = other.m_ptr;
  }

  template <typename U>
  ScopedRefPtr<T>& operator=(const ScopedRefPtr<U>& other) {
    return *this = other.get();
  }

protected:
  T* m_ptr;
};

template <typename T, typename... Args>
inline ScopedRefPtr<T> makeScopedRefPtr(Args&&... args) {
  return ScopedRefPtr<T>{new T(std::forward<Args>(args)...)};
}

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REF_H_
