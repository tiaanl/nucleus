// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

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
inline T* vectorAsArray(std::vector<T>* v, usize requiredSize) {
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
inline T* vectorAsArray(const std::vector<T>* v, usize requiredSize) {
  DCHECK(requiredSize > 0);
  v->reserve(requiredSize);
  v->resize(requiredSize);
  return &*v->begin();
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_STL_H_
