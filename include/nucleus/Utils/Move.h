
#ifndef NUCLEUS_UTILS_MOVE_H_
#define NUCLEUS_UTILS_MOVE_H_

#include "nucleus/Utils/TypeTraits.h"

namespace nu {

// Forwards the argument to another function exactly as it was passed to the calling function.
//
// Example:
//
//   template <typename T>
//   void WrapperFunction(T&& arg) {
//       foo(forward<T>(arg));
//   }
//
//   template <typename... Args>
//   void WrapperFunction(Args&&... args) {
//       foo(forward<Args>(args)...);
//   }
//
template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type& x) {
  return static_cast<T&&>(x);
}

template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type&& x) {
  return static_cast<T&&>(x);
}

// Signals that the type should be moved to it's destination.
//
// Example:
//
//   DynamicArray<String> a;
//   a.pushBack(String("hello"));  // Copies the temporary value into the array.
//
//   a.pushBack(move(String("hello")));  // Moves the insides of the object into place inside the array.
//
template <typename T>
constexpr typename RemoveReference<T>::Type&& move(T&& x) {
  return static_cast<typename RemoveReference<T>::Type&&>(x);
}

// Move a range of items between [first, last) to the destination.
template <typename InputIterator, typename OutputIterator>
inline OutputIterator move(InputIterator first, InputIterator last, OutputIterator destinationFirst) {
  while (first != last) {
    *destinationFirst++ = move(*first);
    first++;
  }
  return destinationFirst;
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_MOVE_H_
