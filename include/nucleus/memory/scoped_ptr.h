#pragma once

#include <utility>

#include "nucleus/macros.h"

namespace nu {

template <typename T>
class ScopedPtr {
public:
  NU_DELETE_COPY(ScopedPtr);

  constexpr ScopedPtr() noexcept : ptr_{nullptr} {}

  constexpr ScopedPtr(T* other) : ptr_{other} {}

  ScopedPtr(ScopedPtr&& other) noexcept : ptr_{other.ptr_} {
    other.ptr_ = nullptr;
  }

  template <typename U>
  ScopedPtr(ScopedPtr<U>&& other) noexcept : ptr_{other.release()} {}

  ~ScopedPtr() {
    // No need to check for `nullptr` before `delete`.
    delete ptr_;
  }

  ScopedPtr& operator=(ScopedPtr&& other) noexcept {
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;

    return *this;
  }

  explicit operator bool() const {
    return !is_null();
  }

  bool operator==(const ScopedPtr& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator==(T* other) const {
    return ptr_ == other;
  }

  bool operator!=(const ScopedPtr& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator!=(const T* other) const {
    return ptr_ != other;
  }

  const T* operator->() const {
    return ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T& operator*() const {
    DCHECK(ptr_);
    return *ptr_;
  }

  T& operator*() {
    DCHECK(ptr_);
    return *ptr_;
  }

  bool is_null() const {
    return ptr_ == nullptr;
  }

  const T* get() const {
    return ptr_;
  }

  T* get() {
    return ptr_;
  }

  template <typename U>
  const U* cast() const {
    return static_cast<const U*>(ptr_);
  }

  template <typename U>
  U* cast() {
    return static_cast<U*>(ptr_);
  }

  void reset(T* p = nullptr) {
    // No need to check for `nullptr` before `delete`.
    delete ptr_;
    ptr_ = p;
  }

  T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

private:
  T* ptr_;
};

template <typename T, typename... Args>
inline ScopedPtr<T> make_scoped_ptr(Args&&... args) {
  return ScopedPtr<T>{new T{std::forward<Args>(args)...}};
}

}  // namespace nu
