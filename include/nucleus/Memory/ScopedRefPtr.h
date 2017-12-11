
#ifndef NUCLEUS_MEMORY_SCOPEDREFPTR_H_
#define NUCLEUS_MEMORY_SCOPEDREFPTR_H_

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

  void swap(T** pptr) {
    T* ptr = m_ptr;
    m_ptr = *pptr;
    *pptr = ptr;
  }

  void swap(ScopedRefPtr<T>& other) {
    swap(&other.m_ptr);
  }

protected:
  T* m_ptr;
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_SCOPEDREFPTR_H_
