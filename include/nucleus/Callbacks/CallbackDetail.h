
#ifndef NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_
#define NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_

#include "nucleus/Macros.h"
#include "nucleus/Memory/RefCountedPtr.h"
#include "nucleus/Memory/ScopedRefPtr.h"

namespace nu {

namespace detail {

class BindStateBase : public RefCountedPtr<BindStateBase> {
  DELETE_COPY_AND_MOVE(BindStateBase);

public:
  using InvokeFuncStorage = void (*)();

private:
  friend struct nu::detail::DeletingRefCountedTraits<BindStateBase>;
  friend class CallbackBase;

  template <typename Functor, typename... BoundArgs>
  friend struct BindState;

  BindStateBase(InvokeFuncStorage polymorphicInvoke);

  ~BindStateBase();

  InvokeFuncStorage m_polymorphicInvoke;
};

class CallbackBase {
public:
  bool isEmpty() const {
    return !m_bindState;
  }

  explicit operator bool() const {
    return !isEmpty();
  }

  void reset();

protected:
  using InvokeFuncStorage = BindStateBase::InvokeFuncStorage;

  CallbackBase() = default;

  ~CallbackBase();

  bool equalsBase(const CallbackBase& other) const;

  explicit CallbackBase(BindStateBase* bindState);

  InvokeFuncStorage getPolymorphicInvoke() const {
    return m_bindState->m_polymorphicInvoke;
  }

  ScopedRefPtr<BindStateBase> m_bindState;
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_
