#pragma once

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"

namespace nu {

template <typename>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)> {
  NU_DELETE_COPY(Function);

public:
  Function() = default;

  Function(Function&& other) : wrapper_{other.wrapper_} {
    other.wrapper_ = nullptr;
  }

  template <typename Callable,
            typename = std::void_t<decltype(std::declval<Callable>()(std::declval<Args>()...))>>
  Function(Callable&& callable)
    : wrapper_{new Wrapper<Callable>(std::forward<Callable>(callable))} {}

  ~Function() {
    reset();
  }

  Function& operator=(Function&& other) {
    wrapper_ = other.wrapper_;
    other.wrapper_ = nullptr;

    return *this;
  }

  void reset() {
    delete wrapper_;
    wrapper_ = nullptr;
  }

  NU_NO_DISCARD bool empty() const {
    return wrapper_;
  }

  Ret operator()(Args... args) const {
    return wrapper_->invoker(wrapper_, std::forward<Args>(args)...);
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

  WrapperBase* wrapper_ = nullptr;
};

}  // namespace nu
