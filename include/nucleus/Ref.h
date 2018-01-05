
#ifndef NUCLEUS_MEMORY_REF_H_
#define NUCLEUS_MEMORY_REF_H_

namespace nu {

template <typename T>
class Ref {
public:
  Ref() : m_ptr(nullptr) {}

  Ref(T* ptr) : m_ptr(ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  Ref(const Ref<T>& other) : m_ptr(other.m_ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  template <typename U>
  Ref(const Ref<U>& other) : m_ptr(other.get()) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  ~Ref() {
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

  Ref<T>& operator=(T* ptr) {
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

  Ref<T>& operator=(const Ref<T>& other) {
    return *this = other.m_ptr;
  }

  template <typename U>
  Ref<T>& operator=(const Ref<U>& other) {
    return *this = other.get();
  }

protected:
  T* m_ptr;
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_REF_H_
