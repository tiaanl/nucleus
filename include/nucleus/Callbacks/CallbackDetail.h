
#ifndef NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_
#define NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_

#include "nucleus/Macros.h"
#include "nucleus/Ref.h"
#include "nucleus/RefCounted.h"

namespace nu {

namespace detail {

class BindStateBase : public RefCounted<BindStateBase> {
public:
  COPY_DELETE(BindStateBase);
  MOVE_DELETE(BindStateBase);

  using InvokeFuncStorage = void (*)();

private:
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

  Ref<BindStateBase> m_bindState;
};

}  // namespace detail

}  // namespace nu

#endif  // NUCLEUS_CALLBACKS_CALLBACK_DETAIL_H_
