
#ifndef NUCLEUS_SINGLETON_H_
#define NUCLEUS_SINGLETON_H_

#include <atomic>

namespace nu {

template <typename T>
class Singleton {
private:
  // Classes using the Singleton<T> pattern should declare a `getInstance()` function and call
  // `Singleton::get()` from within it.
  friend T& T::getInstance();

  // Return a reference to the one true instance of this class.
  static T& get() {
    return s_instance;
  }

  static T s_instance;
};

// static
template <typename T>
T Singleton<T>::s_instance;

}  // namespace nu

#endif  // NUCLEUS_SINGLETON_H_
