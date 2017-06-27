
#ifndef NUCLEUS_ALGORITHMS_COPY_H_
#define NUCLEUS_ALGORITHMS_COPY_H_

namespace nu {

// Copies data from the range [first, last) to [dst, dst + (last - first)).  It
// requires the range [dst, dst + (last - first)) to be valid.  It also requires
// dst not to be in the range [first, last).
template <typename In, typename Out>
Out copy(In first, In last, Out dst) {
    while (first != last) {
        *dst++ = *first++;
    }
    return dst;
};

// Copies data from the range [first, last) to [dst, dst + (last - first)). It
// requires the range [dst, dst + (last - first)) to be valid and uninitialized.
template <typename In, typename T>
T* uninitializedCopy(In first, In last, T* dst) {
    while (first != last) {
        new ((void*) dst++) T(*first++);
    }
    return dst;
};

}  // namespace nu

#endif  // NUCLEUS_ALGORITHMS_COPY_H_
