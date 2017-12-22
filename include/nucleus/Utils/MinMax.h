
#ifndef NUCLEUS_UTILS_MIN_MAX_H_
#define NUCLEUS_UTILS_MIN_MAX_H_

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

#endif  // NUCLEUS_UTILS_MIN_MAX_H_
