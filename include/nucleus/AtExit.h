
#ifndef NUCLEUS_AT_EXIT_H_
#define NUCLEUS_AT_EXIT_H_

#include <stack>

namespace nu {

class AtExit {
public:
  using AtExitCallbackType = void (*)(void*);

  AtExit();

  // The destructor calls all the registered callbacks.  Do not register any more callbacks after
  // this point.
  ~AtExit();

  // Register the specified callback to be called at exit.
  static void registerCallback(AtExitCallbackType callback, void* param);

  // Call the callbacks registered with `registerCallback` in LIFO order.  If is possible to
  // register new callbacks after calling this function.
  static void processCallbacksNow();

protected:
  // This constructor will allow this instance of `AtExit` to be created even if one already exists.
  // This should only be used for testing!  `AtExit` instances are kept on a global stack and it
  // will be removed during destruction.  This allows you to shadow another `AtExit`.
  explicit AtExit(bool shadow);

private:
  struct CallbackAndParam {
    AtExitCallbackType callback;
    void* param;

    void invoke();
  };

  using StackType = std::stack<CallbackAndParam>;

  StackType m_callbacks;

  // Stack of instances to allow shadowing.
  AtExit* m_nextAtExit = nullptr;
};

#if defined(TESTING)
class ShadowAtExit : public AtExit {
public:
  ShadowAtExit() : AtExit(true) {}
};
#endif  // defined(TESTING)

}  // namespace nu

#endif  // NUCLEUS_AT_EXIT_H_
