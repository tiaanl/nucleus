
#ifndef NUCLEUS_UTILS_TYPETRAITS_H_
#define NUCLEUS_UTILS_TYPETRAITS_H_

namespace nu {

// RemoveReference<>

// For a given type T, RemoveReference<T&> or RemoveReference<R&&> is equivalent to T.

template <typename T>
struct RemoveReference {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using Type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using Type = T;
};

}  // namespace nu

#endif  // NUCLEUS_UTILS_TYPETRAITS_H_
