
#include "nucleus/Callbacks/CallbackDetail.h"

#include "nucleus/Logging.h"

namespace nu {

namespace detail {

BindStateBase::BindStateBase(InvokeFuncStorage polymorphicInvoke) : m_polymorphicInvoke(polymorphicInvoke) {}

BindStateBase::~BindStateBase() = default;

void CallbackBase::reset() {
  // Always do this last, because we might be the last ref of object that owns us.
  m_bindState = nullptr;
}

CallbackBase::~CallbackBase() = default;

bool CallbackBase::equalsBase(const CallbackBase& other) const {
  return m_bindState == other.m_bindState;
}

CallbackBase::CallbackBase(BindStateBase* bindState) : m_bindState(bindState) {
  DCHECK(!m_bindState.get() || m_bindState->hasOneRef());
}

}  // namespace detail

}  // namespace nu
