
#ifndef NUCLEUS_UTILS_COPY_H_
#define NUCLEUS_UTILS_COPY_H_

namespace nu {

// Copies data from the range [first, last) to [destinationFirst, destinationFirst + (last - first)).  It requires the
// range [dst, dst + (last - first)) to be valid.  It also requires destinationFirst not to be in the range
// [first, last).
template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator destinationFirst) {
  while (first != last) {
    *destinationFirst++ = *first;
    ++first;
  }
  return destinationFirst;
};

// Copies data from the range [first, last) to [dst, dst + (last - first)). It requires the range
// [dst, dst + (last - first)) to be valid and uninitialized.
template <typename InputIterator, typename T>
T* copyConstruct(InputIterator first, InputIterator last, T* dst) {
  while (first != last) {
    new (*dst++) T(*first++);
  }
  return dst;
};

}  // namespace nu

#endif  // NUCLEUS_UTILS_COPY_H_
