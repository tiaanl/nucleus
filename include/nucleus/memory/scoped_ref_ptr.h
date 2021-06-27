#pragma once

#include <utility>

#include "nucleus/macros.h"

namespace nu {

template <typename T>
class ScopedRefPtr {
public:
  constexpr ScopedRefPtr() = default;

  ScopedRefPtr(T* ptr) : ptr_(ptr) {
    if (ptr_) {
      ptr->addRef();
    }
  }

  ScopedRefPtr(const ScopedRefPtr& other) : ScopedRefPtr{other.ptr_} {}

  ScopedRefPtr(ScopedRefPtr&& other) noexcept : ptr_{other.ptr_} {
    other.ptr_ = nullptr;
  }

  template <typename U,
            typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  ScopedRefPtr(const ScopedRefPtr<U>& other) : ScopedRefPtr(other.ptr_) {}

  ~ScopedRefPtr() {
    if (ptr_) {
      ptr_->release();
    }
  }

  ScopedRefPtr& operator=(const ScopedRefPtr& other) {
    return *this = other.ptr_;
  }

  ScopedRefPtr& operator=(ScopedRefPtr&& other) noexcept {
    std::swap(ptr_, other.ptr_);

    return *this;
  }

  template <typename U>
  ScopedRefPtr& operator=(const ScopedRefPtr<U>& other) {
    return *this = other.get();
  }

  ScopedRefPtr& operator=(T* ptr) {
    // addRef() first so that self assignment works.
    if (ptr) {
      ptr->addRef();
    }

    T* old = ptr_;
    ptr_ = ptr;

    if (old) {
      old->release();
    }

    return *this;
  }

  explicit operator bool() const {
    return !is_null();
  }

  bool is_null() const {
    return ptr_ == nullptr;
  }

  T* get() const {
    return ptr_;
  }

  explicit operator T*() const {
    DCHECK(ptr_);
    return ptr_;
  }

  T* operator->() const {
    DCHECK(ptr_);
    return ptr_;
  }

  //  // Unified assignment operator.
  //  ScopedRefPtr& operator=(ScopedRefPtr other) noexcept {
  //    swap(other);
  //    return *this;
  //  }

  void reset() {
    ScopedRefPtr{}.swap(*this);
  }

  NU_NO_DISCARD T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void swap(ScopedRefPtr& other) noexcept {
    std::swap(ptr_, other.ptr_);
  }

  template <typename U>
  bool operator==(const ScopedRefPtr<U>& right) const {
    return ptr_ == right.get();
  }

  template <typename U>
  bool operator!=(const ScopedRefPtr<U>& right) const {
    return !operator==(right);
  }

  template <typename U>
  bool operator<(const ScopedRefPtr<U>& right) const {
    return ptr_ < right.get();
  }

protected:
  T* ptr_ = nullptr;

private:
  template <typename U>
  friend class ScopedRefPtr;
};

template <typename T, typename... Args>
inline ScopedRefPtr<T> makeScopedRefPtr(Args&&... args) {
  return ScopedRefPtr<T>{new T(std::forward<Args>(args)...)};
}

template <typename T>
bool operator==(const ScopedRefPtr<T>& left, T* right) {
  return left.get() == right;
}

template <typename T>
bool operator!=(const ScopedRefPtr<T>& left, T* right) {
  return left.get() != right;
}

}  // namespace nu
