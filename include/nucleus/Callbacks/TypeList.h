
#ifndef NUCLEUS_CALLBACKS_TYPE_LIST_H_
#define NUCLEUS_CALLBACKS_TYPE_LIST_H_

namespace nu {

namespace detail {

template <typename... Types>
struct TypeList {};

template <size_t N, typename List>
struct DropTypeListItemImpl;

template <size_t N, typename T, typename... List>
struct DropTypeListItemImpl<N, TypeList<T, List...>> : DropTypeListItemImpl<N - 1, TypeList<List...>> {};

template <typename T, typename... List>
struct DropTypeListItemImpl<0, TypeList<T, List...>> {
  using Type = TypeList<T, List...>;
};

template <>
struct DropTypeListItemImpl<0, TypeList<>> {
  using Type = TypeList<>;
};

template <size_t N, typename List>
using DropTypeListItem = typename DropTypeListItemImpl<N, List>::Type;

template <size_t N, typename List, typename... Accum>
struct TakeTypeListItemImpl;

template <size_t N, typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<N, TypeList<T, List...>, Accum...>
  : TakeTypeListItemImpl<N - 1, TypeList<List...>, Accum..., T> {};

template <typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, Accum...> {
  using Type = TypeList<Accum...>;
};

template <typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<>, Accum...> {
  using Type = TypeList<Accum...>;
};

// `TakeTypeListItem<3, TypeList<A, B, C, D>>` is evaluated to `TypeList<A, B, C>`.
template <size_t N, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<N, List>::Type;

template <typename List1, typename List2>
struct ConcatTypeListsImpl;

template <typename... Types1, typename... Types2>
struct ConcatTypeListsImpl<TypeList<Types1...>, TypeList<Types2...>> {
  using Type = TypeList<Types1..., Types2...>;
};

template <typename List1, typename List2>
using ConcatTypeLists = typename ConcatTypeListsImpl<List1, List2>::Type;

}  //  namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_TYPE_LIST_H_
