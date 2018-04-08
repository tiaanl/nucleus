
#ifndef NUCLEUS_CALLBACKS_FUNCTOR_TRAITS_H_
#define NUCLEUS_CALLBACKS_FUNCTOR_TRAITS_H_

#include <type_traits>

#include "nucleus/Config.h"

namespace nu {

namespace detail {

template <typename Functor, typename SFINAE = void>
struct FunctorTraits;

// Functions
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)> {
  using RunType = R(Args...);
  static constexpr bool isMethod = false;
  static constexpr bool isNullable = true;

  template <typename... RunArgs>
  static R invoke(R (*function)(Args...), RunArgs&&... args) {
    return function(std::forward<RunArgs>(args)...);
  }
};

#if OS(WIN) && !ARCH(CPU_X86_64)

// Functions
template <typename R, typename... Args>
struct FunctorTraits<R(__stdcall*)(Args...)> {
  using RunType = R(Args...);
  static constexpr bool isMethod = false;
  static constexpr bool isNullable = true;

  template <typename... RunArgs>
  static R invoke(R(__stdcall* function)(Args...), RunArgs&&... args) {
    return function(std::forward<RunArgs>(args)...);
  }
};

// Functions
template <typename R, typename... Args>
struct FunctorTraits<R(__fastcall*)(Args...)> {
  using RunType = R(Args...);
  static constexpr bool isMethod = false;
  static constexpr bool isNullable = true;

  template <typename... RunArgs>
  static R invoke(R(__fastcall* function)(Args...), RunArgs&&... args) {
    return function(std::forward<RunArgs>(args)...);
  }
};

#endif  // OS(WIN) && !ARCH(CPU_X86_64)

// Methods
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...)> {
  using RunType = R(Receiver*, Args...);
  static constexpr bool isMethod = true;
  static constexpr bool isNullable = true;

  template <typename ReceiverPtr, typename... RunArgs>
  static R invoke(R (Receiver::*method)(Args...), ReceiverPtr&& receiverPtr, RunArgs&&... args) {
    return ((*receiverPtr).*method)(std::forward<RunArgs>(args)...);
  }
};

// Const Methods
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...) const> {
  using RunType = R(const Receiver*, Args...);
  static constexpr bool isMethod = true;
  static constexpr bool isNullable = true;

  template <typename ReceiverPtr, typename... RunArgs>
  static R invoke(R (Receiver::*method)(Args...) const, ReceiverPtr&& receiverPtr,
                  RunArgs&&... args) {
    return ((*receiverPtr).*method)(std::forward<RunArgs>(args)...);
  }
};

// Callbacks
template <typename R, typename... Args>
struct FunctorTraits<Callback<R(Args...)>> {
  using RunType = R(Args...);
  static constexpr bool isMethod = false;
  static constexpr bool isNullable = true;

  template <typename CallbackType, typename... RunArgs>
  static R invoke(CallbackType&& callback, RunArgs&&... args) {
    DCHECK(!callback.isEmpty());
    return std::forward<CallbackType>(callback).run(std::forward<RunArgs>(args)...);
  }
};

template <typename Functor>
using MakeFunctorTraits = FunctorTraits<std::decay_t<Functor>>;

template <typename Functor>
std::enable_if_t<FunctorTraits<Functor>::isNullable, bool> isNull(const Functor& functor) {
  return !functor;
}

template <typename Functor>
std::enable_if_t<!FunctorTraits<Functor>::isNullable, bool> isNull(const Functor&) {
  return false;
}

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_FUNCTOR_TRAITS_H_
