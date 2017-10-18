
#ifndef NUCLEUS_UTILS_MEMORY_H_
#define NUCLEUS_UTILS_MEMORY_H_

namespace nu {

template <typename ForwardIterator, typename Count>
inline void uninitializedDefaultFillN(ForwardIterator first, Count n) {
  using ValueType = IteratorTraits<ForwardIterator>::ValueType;
  ForwardIterator currentDest(first);

  for (; n > 0; --n, ++currentDest) {
    ::new (static_cast<void*>(&*currentDest)) ValueType();
  }
};

namespace detail {

template <typename ForwardIterator, typename Count, typename T>
inline void uninitializedFillNImpl(ForwardIterator first, Count n, const T& value, TrueType) {
  fillN(first, n, value);
};

template <typename ForwardIterator, typename Count, typename T>
inline void uninitializedFillNImpl(ForwardIterator first, Count n, const T& value, FalseType) {
  using ValueType = IteratorTraits<ForwardIterator>::valueType;
  ForwardIterator currentDest(first);

  for (; n > 0; --n, ++currentDest) {
    ::new ((void*)&*currentDest) ValueType(value);
  }
};

}  // namespace detail

template <typename T, typename Count>
inline void uninitializedFillNPtr(T* first, Count n, const T& value) {
  using ValueType = typename IteratorTraits<GenericIterator<T*, void>>::ValueType;
  detail::uninitializedFillNImpl(GenericIterator<T*, void>(first), n, value, isTriviallyCopyAssignable<ValueType>());
};

}  // namespace nu

#endif  // NUCLEUS_UTILS_MEMORY_H_
