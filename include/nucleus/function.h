#pragma once

#include "nucleus/Memory/scoped_ptr.h"
#include "nucleus/logging.h"
#include "nucleus/macros.h"

namespace nu {

template <typename>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)> {
  NU_DELETE_COPY(Function);

public:
  NU_DEFAULT_MOVE(Function);

  Function() = default;

  template <typename Callable,
            typename = std::void_t<decltype(std::declval<Callable>()(std::declval<Args>()...))>>
  Function(Callable&& callable)
    : wrapper_{makeScopedPtr<Wrapper<Callable>>(std::forward<Callable>(callable))} {}

  ~Function() {
    reset();
  }

  void reset() {
    wrapper_ = nullptr;
  }

  NU_NO_DISCARD bool empty() const {
    return wrapper_;
  }

  Ret operator()(Args... args) const {
    return wrapper_->invoker(const_cast<WrapperBase*>(wrapper_.get()), std::forward<Args>(args)...);
  }

private:
  struct WrapperBase {
    Ret (*invoker)(WrapperBase*, Args...);
  };

  template <typename Callable>
  struct Wrapper : WrapperBase {
    std::decay_t<Callable> callable;

    static Ret invoker(WrapperBase* wrapper, Args... args) {
      return static_cast<Wrapper*>(wrapper)->callable(std::forward<Args>(args)...);
    }

    template <typename T>
    Wrapper(T&& callable) : WrapperBase{&invoker}, callable{std::forward<T>(callable)} {}

  private:
    NU_DELETE_COPY(Wrapper);
  };

  ScopedPtr<WrapperBase> wrapper_;
};

}  // namespace nu
