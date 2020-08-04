#if !defined(NUCLEUS_CALLBACKS_DETAIL_TYPE_LIST_H_)
#define NUCLEUS_CALLBACKS_DETAIL_TYPE_LIST_H_

namespace nu::detail {

template <typename... Types>
struct TypeList {};

template <size_t N, typename List>
struct DropTypeListItemImpl;

template <size_t N, typename T, typename... List>
struct DropTypeListItemImpl<N, TypeList<T, List...>>
  : DropTypeListItemImpl<N - 1, TypeList<List...>> {};

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

template <size_t N, typename List, typename... Accumulator>
struct TakeTypeListItemImpl;

template <size_t N, typename T, typename... List, typename... Accumulator>
struct TakeTypeListItemImpl<N, TypeList<T, List...>, Accumulator...>
  : TakeTypeListItemImpl<N - 1, TypeList<List...>, Accumulator..., T> {};

template <typename T, typename... List, typename... Accumulator>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, Accumulator...> {
  using Type = TypeList<Accumulator...>;
};

template <typename... Accumulator>
struct TakeTypeListItemImpl<0, TypeList<>, Accumulator...> {
  using Type = TypeList<Accumulator...>;
};

template <size_t N, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<N, List>::Type;

}  // namespace nu::detail

#endif  // NUCLEUS_CALLBACKS_DETAIL_TYPE_LIST_H_
