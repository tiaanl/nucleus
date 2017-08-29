
#ifndef NUCLEUS_UTILS_MOVE_H_
#define NUCLEUS_UTILS_MOVE_H_

#include "nucleus/utils/TypeTraits.h"

namespace nu {

// forward

// Forwards the argument to another function exactly as it was passed to the calling function.  Not to be confused with
// |move|, which is specifically for echoing templated argument types to another function.  |move| is specifically about
// making a type be an rvalue reference (i.e. movable) type.
//
// Example:
//
//   template <typename T>
//   void WrapperFunction(T&& arg) {
//       foo(nu::forward<T>(arg));
//   }
//
//   template <typename... Args>
//   void WrapperFunction(Args&&... args) {
//       foo(nu::forward<Args>(args)...);
//   }

template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type& x) {
    return static_cast<T&&>(x);
}

template <typename T>
constexpr T&& forward(typename RemoveReference<T>::Type&& x) {
    return static_cast<T&&>(x);
}

// move

// |move| obtains an rvalue reference to its argument and converts it to an xvalue.
// Returns, by definition: static_cast<typename RemoveReference<T>::Type&&>(t).
// The primary use of this is to pass a move'd type to a function which takes T&& and thus select that function instead
// of a function which takes T or T&.

template <typename T>
constexpr typename RemoveReference<T>::Type&& move(T&& x) {
    return static_cast<typename RemoveReference<T>::Type&&>(x);
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_MOVE_H_
