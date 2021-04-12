#pragma once

#include "nucleus/Logging.h"
#include "nucleus/Macros.h"
#include "nucleus/Memory/ScopedPtr.h"

namespace nu {

template <typename Signature>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)> {
public:
  Function() = default;

  template <typename Functor>
  Function(Functor&& functor)
    : wrapper_{makeScopedPtr<Wrapper<Functor>>(std::forward<Functor>(functor))} {}

  Ret operator()(Args... args) const {
    DCHECK(wrapper_);
    return wrapper_->invoke(std::forward<Args>(args)...);
  }

private:
  class WrapperBase {
  public:
    virtual ~WrapperBase() = default;
    virtual Ret invoke(Args...) const = 0;
  };

  template <typename Functor>
  class Wrapper final : public WrapperBase {
    NU_DELETE_COPY(Wrapper);

  public:
    explicit Wrapper(Functor&& functor) : functor_{std::move(functor)} {}

    Ret invoke(Args... args) const final {
      if constexpr (requires { functor_(std::forward<Args>(args)...); }) {
        return functor_(std::forward<Args>(args)...);
      } else if constexpr (requires { functor_(); }) {
        return functor_();
      } else if constexpr (std::is_same_v<void, Ret>) {
        return;
      } else {
        return {};
      }
    }

  private:
    Functor functor_;
  };

  ScopedPtr<WrapperBase> wrapper_;
};

}  // namespace nu
