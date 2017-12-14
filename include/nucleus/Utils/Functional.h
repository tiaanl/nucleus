
#ifndef NUCLEUS_FUNCTIONAL_H_
#define NUCLEUS_FUNCTIONAL_H_

#include "nucleus/Utils/Move.h"

namespace nu {

// Defines a function that takes two arguments and returns a single result.
template <typename Arg1, typename Arg2, typename Result>
struct BinaryFunction {
  using FirstArgumentType = Arg1;
  using SecondArgumentType = Arg2;
  using ResultType = Result;
};

// Less comparison of two values.
template <typename T = void>
struct Less : public BinaryFunction<T, T, bool> {
  bool operator()(const T& left, const T& right) const {
    return left < right;
  }
};

template <>
struct Less<void> {
  template <typename LeftType, typename RightType>
  auto operator()(LeftType&& left, RightType&& right) const
      -> decltype(forward<LeftType>(left) < forward<RightType>(right)) {
    return forward<LeftType>(left) < forward<RightType>(right);
  }
};

}  // namespace nu

#endif  // NUCLEUS_FUNCTIONAL_H_
