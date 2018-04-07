
#include "nucleus/AtExit.h"

#include "nucleus/Logging.h"

namespace nu {

namespace {

// This is the top-most `AtExit` instance.
static AtExit* g_topAtExit = nullptr;

}  // namespace

AtExit::AtExit() : m_nextAtExit(g_topAtExit) {
  g_topAtExit = this;
}

AtExit::~AtExit() {
  if (!g_topAtExit) {
    NOTREACHED() << "Tried to ~AtExit without an AtExit";
    return;
  }

  // The `AtExit` instance should be the top most one.
  DCHECK(g_topAtExit == this);

  processCallbacksNow();

  // From now on we operator on the next instance.
  g_topAtExit = m_nextAtExit;
}

// static
void AtExit::registerCallback(AtExitCallbackType callback, void* param) {
  DCHECK(callback);
  g_topAtExit->m_callbacks.push({callback, param});
}

// static
void AtExit::processCallbacksNow() {
  if (!g_topAtExit) {
    NOTREACHED() << "Tried to processCallbacksNow without an AtExit";
    return;
  }

  StackType callbacks = std::move(g_topAtExit->m_callbacks);

  while (!callbacks.empty()) {
    CallbackAndParam callback = callbacks.top();
    callback.invoke();
    callbacks.pop();
  }

  DCHECK(g_topAtExit->m_callbacks.empty());
}

AtExit::AtExit(bool shadow) : m_nextAtExit(g_topAtExit) {
  DCHECK(shadow || !g_topAtExit);
  g_topAtExit = this;
}

void AtExit::CallbackAndParam::invoke() {
  callback(param);
}

}  // namespace nu