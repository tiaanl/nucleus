
#ifndef NUCLEUS_UTILS_SIGNALS_H_
#define NUCLEUS_UTILS_SIGNALS_H_

#include <functional>

#include "nucleus/logging.h"
#include "nucleus/macros.h"
#include "nucleus/types.h"

namespace nu {

namespace detail {

// ProtoSignal is the template implementation for callback lists.
template <typename Signature, typename Collector>
class ProtoSignal;

// CollectorInvocation invokes signal handlers differently depending on return
// type.
template <typename, typename>
struct CollectorInvocation;

// CollectorLast returns the result of the last signal handler from a signal
// emission.
template <typename Result>
struct CollectorLast {
  using CollectorResult = Result;

  CollectorLast() : m_last() {}

  bool operator()(Result r) {
    m_last = r;
    return true;
  }

  CollectorResult getResult() {
    return m_last;
  }

private:
  Result m_last;
};

// CollectorDefault implements the default signal handler collection behaviour.
template <typename Result>
struct CollectorDefault : CollectorLast<Result> {};

// CollectorDefault specialization for signals with void return types.
template <>
struct CollectorDefault<void> {
  using CollectorResult = void;

  void getResult() {}

  bool operator()(void) {
    return true;
  }
};

// CollectorInvocation specialization for regular signals.
template <typename Collector, typename R, typename... Args>
struct CollectorInvocation<Collector, R(Args...)> {
  bool invoke(Collector& collector, const std::function<R(Args...)>& callbackFunction, Args... args) {
    return collector(callbackFunction(args...));
  }
};

// CollectorInvocation specialization for signals with void return type.
template <typename Collector, typename... Args>
struct CollectorInvocation<Collector, void(Args...)> {
  bool invoke(Collector& collector, const std::function<void(Args...)>& callbackFunction, Args... args) {
    callbackFunction(args...);
    return collector();
  }
};

// ProtoSignal template specialised for the callback signature and collector.
template <typename Collector, typename R, typename... Args>
class ProtoSignal<R(Args...), Collector> : private CollectorInvocation<Collector, R(Args...)> {
public:
  using CallbackFunction = std::function<R(Args...)>;
  using Result = typename CallbackFunction::result_type;
  using CollectorResult = typename Collector::CollectorResult;

  explicit ProtoSignal(const CallbackFunction& function) {
    if (function) {
      ensureRing();
      m_callbackRing->function = function;
    }
  }

  // Releases all resources associated with this signal.
  ~ProtoSignal() {
    if (m_callbackRing) {
      while (m_callbackRing->next != m_callbackRing) {
        m_callbackRing->next->unlink();
      }
      DCHECK(m_callbackRing->refCount >= 2);
      m_callbackRing->decRef();
      m_callbackRing->decRef();
    }
  }

  // Add a new function or lambda as signal handler, returns a handler
  // connection ID.
  USize connect(const CallbackFunction& function) {
    ensureRing();
    return m_callbackRing->addBefore(function);
  }

  // Operator to remove a signal handler through it's connection ID, returns if
  // a handler was removed.
  bool disconnect(USize connection) {
    return m_callbackRing ? m_callbackRing->removeSiblings(connection) : false;
  }

  // Emit a signal, i.e. invoke it's callbacks and collect return types with the
  // Collector.
  CollectorResult emit(Args... args) {
    Collector collector;

    if (!m_callbackRing) {
      return collector.getResult();
    }

    SignalLink* link = m_callbackRing;
    link->incRef();
    do {
      if (link->function) {
        if (!this->invoke(collector, link->function, args...)) {
          break;
        }
      }
      SignalLink* old = link;
      link = old->next;
      link->incRef();
      old->decRef();
    } while (link != m_callbackRing);
    link->decRef();
    return collector.getResult();
  }

  // Return the number of connected slots.
  USize getSize() {
    USize size = 0;
    SignalLink* link = m_callbackRing;
    link->incRef();
    do {
      if (link->function) {
        ++size;
      }
      SignalLink* old = link;
      link = old->next;
      link->incRef();
      old->decRef();
    } while (link != m_callbackRing);

    return size;
  }

private:
  // SignalLink implements a doubly-linked ring with ref-counted nodes
  // containing the signal handlers.
  struct SignalLink {
    SignalLink* next{nullptr};
    SignalLink* prev{nullptr};
    CallbackFunction function;
    I32 refCount{1};

    explicit SignalLink(const CallbackFunction& function) : function(function) {}

    ~SignalLink() {
      DCHECK(refCount == 0);
    }

    void incRef() {
      ++refCount;
      DCHECK(refCount > 0);
    }

    void decRef() {
      --refCount;
      if (!refCount) {
        delete this;
      } else {
        DCHECK(refCount > 0);
      }
    }

    void unlink() {
      function = nullptr;
      if (next) {
        next->prev = prev;
      }
      if (prev) {
        prev->next = next;
      }
      decRef();
    }

    USize addBefore(const CallbackFunction& callbackFunction) {
      SignalLink* link = new SignalLink(callbackFunction);
      link->prev = prev;
      link->next = this;
      prev->next = link;
      prev = link;

      static_assert(sizeof(link) == sizeof(USize), "sizeof(usize)");

      return reinterpret_cast<USize>(link);
    }

    bool deactivate(const CallbackFunction& callbackFunction) {
      if (callbackFunction == function) {
        function = nullptr;
        return true;
      }

      for (SignalLink* link = next ? next : this; link != this; link = link->next) {
        if (callbackFunction == link->function) {
          link->unlink();
          return true;
        }
      }

      return false;
    }

    bool removeSiblings(USize id) {
      for (SignalLink* link = next ? next : this; link != this; link = link->next) {
        if (id == reinterpret_cast<USize>(link)) {
          link->unlink();
          return true;
        }
      }
      return false;
    }
  };

  void ensureRing() {
    if (!m_callbackRing) {
      m_callbackRing = new SignalLink(CallbackFunction());
      m_callbackRing->incRef();
      m_callbackRing->next = m_callbackRing;
      m_callbackRing->prev = m_callbackRing;
    }
  }

  SignalLink* m_callbackRing{nullptr};

  DISALLOW_COPY_AND_ASSIGN(ProtoSignal);
};

}  // namespace detail

// Signal is a template type providing an interface for arbitrary callback
// lists.  A signal type needs to be declared with the function signature of its
// callbacks, and optionally a return result collector class type.
//
// The callbacks of a signal are invoked with the emit() function and arguments
// according to the signature.  The result returned by emit() depends on the
// signal collector class.  By default, the result of the last callback is
// returned from emit().  Collectors can be implemented to accululate callback
// results or to halt a running emissions in correspondance to callback results.
//
// The signal implementation is safe against recursion, so callbacks may be
// removed and added during a signal emission and recursive emit() calls are
// also safe.
template <typename Signature,
          typename Collector = detail::CollectorDefault<typename std::function<Signature>::result_type>>
struct Signal final : detail::ProtoSignal<Signature, Collector> {
  using ProtoSignal = detail::ProtoSignal<Signature, Collector>;
  using CallbackFunction = typename ProtoSignal::CallbackFunction;

  // Signal constructor, supports a default callback as argument.
  Signal(const CallbackFunction& function = CallbackFunction()) : ProtoSignal(function) {}
};

// This function creates a std::function by binding an object to the member
// function pointer.
template <typename Instance, typename Class, typename R, typename... Args>
std::function<R(Args...)> slot(R (Class::*method)(Args...), Instance& object) {
  return [&object, method](Args... args) { return (object.*method)(args...); };
}

// This function creates a std::function by binding an object to the member
// function pointer.
template <typename Class, typename R, typename... Args>
std::function<R(Args...)> slot(R (Class::*method)(Args...), Class* object) {
  return [object, method](Args... args) { return (object->*method)(args...); };
}

}  // namespace nu

#endif  // NUCLEUS_UTILS_SIGNALS_H_
