
#ifndef NUCLEUS_ALGORITHMS_UTILITY_H_
#define NUCLEUS_ALGORITHMS_UTILITY_H_

namespace nu {

template <typename T>
const T& min(const T& left, const T& right) {
  return (right < left) ? right : left;
}

template <typename T>
const T& max(const T& left, const T& right) {
  return (left < right) ? right : left;
}

}  // namespace nu

#endif  // NUCLEUS_ALGORITHMS_UTILITY_H_
