
#ifndef NUCLEUS_FUNCTION_H_
#define NUCLEUS_FUNCTION_H_

#include "nucleus/Memory/RefCountedPtr.h"
#include "nucleus/Memory/ScopedRefPtr.h"
#include "nucleus/Types.h"

#include <tuple>

namespace nu {

template <typename Signature>
struct FunctionTraits {};

template <typename Ret, typename... Args>
struct FunctionTraits<Ret (*)(Args...)> {
  using ReturnType = Ret;
};

template <MemSize...>
struct Sequence {};

template <MemSize N, MemSize... S>
struct GenerateSequence : GenerateSequence<N - 1, N - 1, S...> {};

template <MemSize... S>
struct GenerateSequence<0, S...> {
  using Type = GenerateSequence<S...>;
};

struct BindStateBase : public RefCountedPtr<BindStateBase> {
  using InvokeFunctionStorageType = void (*)();

  InvokeFunctionStorageType invokeFunctionStorage;

  explicit BindStateBase(InvokeFunctionStorageType invokeFunctionStorage)
    : invokeFunctionStorage(invokeFunctionStorage) {}
};

template <typename ReturnType>
struct InvokeHelper {
  template <typename Function, typename... Args>
  static inline ReturnType makeItSo(Function&& function, Args&&... args) {
    using Traits = MakeFunctorTraits<Functor>;
    return Traits::invoke(std::forward<Functor>(functor), std::forward<RunArgs>(args)...);
  }
};

template <typename BindStateType, typename ReturnType>
struct Invoker {
  static ReturnType invoke(BindStateBase* bindState) {
    const BindStateType* typedBindState = static_cast<BindStateType*>(bindState);

    static constexpr MemSize numBoundArgs = std::tuple_size<decltype(typedBindState->args)>::value;

    return invokeImpl(typedBindState->function, typedBindState->args,
                      std::make_index_sequence<numBoundArgs>());
  }

private:
  template <typename Function, typename Args, MemSize... I>
  static inline ReturnType invokeImpl(Function&& function, Args&& args, std::index_sequence<I...>) {
    return function(std::get<I>(std::forward<Args>(args))...);
  }
};

template <typename Function, typename... Args>
struct BindState : public BindStateBase {
  using InvokeFunctionType =
      Invoker<BindState<Function, Args...>, typename FunctionTraits<Function>::ReturnType>;

  Function function;
  std::tuple<Args...> args;

  BindState(BindStateBase::InvokeFunctionStorageType invokeFunctionStorage, Function function,
            Args&&... args)
    : BindStateBase(invokeFunctionStorage), function(function), args(std::forward<Args>(args)...) {}

private:
  friend struct detail::DeletingRefCountedTraits<BindStateBase>;
};

template <typename Function, typename... Args>
auto makeBindState(Function function, Args&&... args) -> BindState<Function, Args...>* {
  using BindStateType = BindState<Function, Args...>;
  using InvokerType = Invoker<BindStateType, typename FunctionTraits<Function>::ReturnType>;

  auto invokerFunction = &InvokerType::invoke;

  return new BindStateType(
      reinterpret_cast<BindStateBase::InvokeFunctionStorageType>(invokerFunction), function,
      std::forward<Args>(args)...);
}

template <typename ReturnType>
class Callback {
public:
  Callback() = default;

  explicit Callback(BindStateBase* bindState) : m_bindState(bindState) {}

  bool isEmpty() const {
    return m_bindState.isEmpty();
  }

  // Invoke the callback's bound function, passing it the bound arguments.
  template <typename... Args>
  ReturnType invoke(Args&&... args) {
    auto invokerFunction =
        reinterpret_cast<ReturnType (*)(BindStateBase*)>(m_bindState->invokeFunctionStorage);

    return invokerFunction(m_bindState.get());
  }

private:
  ScopedRefPtr<BindStateBase> m_bindState;
};

template <typename Function, typename... Args>
auto bind(Function function, Args&&... args) {
  // Create a new BindState to hold the arguments.
  using ReturnType = FunctionTraits<Function>::ReturnType;
  using BindStateType = BindState<Function, Args...>;
  using InvokerType = Invoker<BindStateType, ReturnType>;

  BindStateType* bindState = makeBindState(function, std::forward<Args>(args)...);

  return Callback<ReturnType>(bindState);
}

}  // namespace nu

#endif  // NUCLEUS_FUNCTION_H_
