
#ifndef NUCLEUS_OPTIONAL_H_
#define NUCLEUS_OPTIONAL_H_

#include <utility>

namespace nu {

namespace detail {

using EmptyType = char;

constexpr EmptyType emptyValue = 0;

}  // namespace detail

template <typename T>
class Optional {
public:
  // Construct default empty value.
  Optional() : m_empty(detail::emptyValue) {}

  // Construct with an initial value.
  explicit Optional(const T& value) : m_value(value), m_valid(true) {}

  // Copy constructor.
  Optional(const Optional& other) : m_value(other.m_value), m_valid(other.m_valid) {}

  // Construct by moving the value.
  Optional(Optional&& other) : m_value(std::move(other.m_value)), m_valid(other.m_valid) {}

  Optional(T&& other) : m_value(std::move(other)), m_valid(true) {}

  ~Optional() {
    // If the value we hold is valid, then we have to destruct it.
    if (m_valid) {
      reset();
    }
  }

  // Assign another Optional to this optional.
  Optional& operator=(const Optional& other) {
    m_value = other.m_value;
    m_valid = other.m_value;

    return *this;
  }

  // Move another Optional into this one.
  Optional& operator=(Optional&& other) {
    m_value = std::move(other.m_value);
    m_valid = std::move(other.m_valid);

    return *this;
  }

  // Reset back to empty value.
  void reset() {
    // We have to manually call the destructor here if applicable.
    if (m_valid && std::is_destructible<T>::value) {
      m_value.~T();
    }

    m_empty = detail::emptyValue;
    m_valid = false;
  }

  // Return a reference to the value we hold.
  T& get() {
    return m_value;
  }
  const T& get() const {
    return m_value;
  }

  // Returns true if the value is valid.
  bool isValid() const {
    return m_valid;
  }

private:
  // The value we're holding.
  union {
    T m_value;
    detail::EmptyType m_empty{detail::emptyValue};
  };

  // Set to true if the value we are holding is valid.
  bool m_valid = false;
};

template <typename T, typename... Args>
Optional<T> makeOptional(Args&&... args) {
  return Optional<T>(T(std::forward<Args>(args)...));
};

template <typename T>
bool operator==(const Optional<T>& left, const T& right) {
  return left.isValid() ? left.get() == right : false;
}

template <typename T>
bool operator==(const T& left, const Optional<T>& right) {
  return right.isValid() ? right.get() == left : false;
}

template <typename T>
bool operator!=(const Optional<T>& left, const T& right) {
  return left.isValid() ? !(left == right) : true;
}

template <typename T>
bool operator!=(const T& left, const Optional<T>& right) {
  return right.isValid() ? !(left == right) : true;
}

}  // namespace nu

#endif  // NUCLEUS_OPTIONAL_H_
