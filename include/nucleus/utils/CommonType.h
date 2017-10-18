
#ifndef NUCLEUS_UTILS_COMMONTYPE_H_
#define NUCLEUS_UTILS_COMMONTYPE_H_

namespace nu {

template <typename... T>
struct CommonTypeImpl;

template <typename T>
struct CommonTypeImpl<T> {
  using Type = T;
};

template <typename T, typename U>
struct CommonTypeImpl<T, U> {
  static T&& t();
  static U&& u();
  using Type = decltype(true ? t() : u());
};

template <typename T, typename U, typename... V>
struct CommonTypeImpl<T, U, V...> {
  using Type = typename CommonTypeImpl<typename CommonTypeImpl<T, U>::Type, V...>::Type;
};

template <typename... T>
using CommonType = typename CommonTypeImpl<T...>::Type;

}  // namespace nu

#endif  // NUCLEUS_UTILS_COMMONTYPE_H_
