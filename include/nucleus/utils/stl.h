
#ifndef NUCLEUS_UTILS_STL_H_
#define NUCLEUS_UTILS_STL_H_

#include <vector>

namespace nu {

// vectorAsArray
// -----------------------------------------------------------------------------
// To treat a possibly-empty vector as an array, use these functions.  The
// variants with required sizes will expand the vector if it can't hold the
// required size before returning the internal pointer.

template <typename T>
inline T* vectorAsArray(std::vector<T>* v) {
  return v->empty() ? nullptr : &*v->begin();
}

template <typename T>
inline T* vectorAsArray(std::vector<T>* v, USize requiredSize) {
  DCHECK(requiredSize > 0U);
  v->reserve(requiredSize);
  v->resize(requiredSize);
  return &*v->begin();
}

template <typename T>
inline const T* vectorAsArray(const std::vector<T>* v) {
  return v->empty() ? nullptr : &*v->begin();
}

template <typename T>
inline T* vectorAsArray(const std::vector<T>* v, USize requiredSize) {
  DCHECK(requiredSize > 0);
  v->reserve(requiredSize);
  v->resize(requiredSize);
  return &*v->begin();
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_STL_H_
