#pragma once

#include "nucleus/Logging.h"
#include "nucleus/Types.h"

namespace nu {

template <typename T>
class Optional {
public:
  Optional() = default;

  template <typename... Args>
  Optional(Args&&... args) : value_is_set_{true} {
    new (data_) T{std::forward<Args>(args)...};
  }

  Optional(const Optional& other) : value_is_set_{other.value_is_set_} {
    if (value_is_set_) {
      new (data_) T{other.value()};
    }
  }

  Optional(Optional&& other) : value_is_set_{other.value_is_set_} {
    if (value_is_set_) {
      new (data_) T{std::move(other.value())};
    }
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
    return *reinterpret_cast<T*>(data_);
  }

  T& value() {
    DCHECK(value_is_set_);
    return *reinterpret_cast<T*>(data_);
  }

private:
  alignas(T) U8 data_[sizeof(T)];
  bool value_is_set_ = false;
};

}  // namespace nu
