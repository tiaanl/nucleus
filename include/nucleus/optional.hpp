#pragma once

#include <ostream>

#include "nucleus/logging.h"
#include "nucleus/types.h"

namespace nu {

template <typename T>
class Optional {
public:
  Optional() : value_is_set_{false} {}

  Optional(const Optional& other) : value_is_set_{other.value_is_set_} {
    if (value_is_set_) {
      new (data_) T{other.value()};
    }
  }

  Optional(const T& value) : value_is_set_{true} {
    new (data_) T{value};
  }

  Optional(Optional&& other) : value_is_set_{other.value_is_set_} {
    if (value_is_set_) {
      new (data_) T{std::move(other.value())};
    }
  }

  Optional(T&& value) : value_is_set_{true} {
    new (data_) T{std::forward<T>(value)};
  }

  ~Optional() {
    if (value_is_set_) {
      reinterpret_cast<T*>(data_)->~T();
    }
  }

  Optional& operator=(const Optional& other) {
    value_is_set_ = other.value_is_set_;
    if (value_is_set_) {
      *reinterpret_cast<T*>(data_) = *reinterpret_cast<const T*>(other.data_);
    }

    return *this;
  }

  Optional& operator=(Optional&& other) noexcept {
    value_is_set_ = other.value_is_set_;
    other.value_is_set_ = false;

    if (value_is_set_) {
      *reinterpret_cast<T*>(data_) =
          std::move(*const_cast<T*>(reinterpret_cast<const T*>(other.data_)));
    }

    return *this;
  }

  bool has_value() const {
    return value_is_set_;
  }

  const T& value() const {
    DCHECK(value_is_set_);
    return *reinterpret_cast<const T*>(data_);
  }

  T& value() {
    DCHECK(value_is_set_);
    return *reinterpret_cast<T*>(data_);
  }

  T& operator*() const {
    return value();
  }

  T& operator*() {
    return value();
  }

  T* operator->() const {
    DCHECK(value_is_set_);
    return reinterpret_cast<const T*>(data_);
  }

  T* operator->() {
    DCHECK(value_is_set_);
    return reinterpret_cast<T*>(data_);
  }

private:
  alignas(T) U8 data_[sizeof(T)];
  bool value_is_set_;
};

}  // namespace nu

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const nu::Optional<T>& value) {
  if (value.has_value()) {
    os << value.value();
  } else {
    os << "(null)";
  }

  return os;
}
