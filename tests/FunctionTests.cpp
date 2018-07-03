
#include "nucleus/Function.h"
#include "nucleus/Logging.h"
#include "nucleus/Testing.h"

namespace nu {

I32 add(I32 a, I32 b) {
  return a + b;
}

TEST_CASE("construct an empty callback") {
  Callback<void> callbackWithVoid;
  CHECK(callbackWithVoid.isEmpty());

  Callback<I32> callbackWithInt;
  CHECK(callbackWithInt.isEmpty());
}

I32 returnValue() {
  return 12345;
}

TEST_CASE("callback can return a value") {
  Callback<I32> callbackReturningInt = bind(&returnValue);
  CHECK(callbackReturningInt.invoke() == 12345);
}

I32 returnPassed(I32 passed) {
  return passed;
}

TEST_CASE("callback can return value it was passed") {
  Callback<I32> callbackReturningPassedInt = bind(&returnPassed, 123);
  CHECK(callbackReturningPassedInt.invoke() == 123);
}

I32 addNumbers(I32 a, I32 b, I32 c) {
  return a + b + c;
}

TEST_CASE("callback can hold more than one bound arguments") {
  Callback<I32> callbackAddingNumbers = bind(&addNumbers, 1, 2, 3);
  CHECK(callbackAddingNumbers.invoke());
}

struct LifetimeTracker {
  static I32 s_defaultConstructed;
  static I32 s_valueConstructed;
  static I32 s_copyConstructed;
  static I32 s_moveConstructed;
  static I32 s_copied;
  static I32 s_moved;
  static I32 s_destructed;

  static void reset() {
    s_defaultConstructed = 0;
    s_valueConstructed = 0;
    s_copyConstructed = 0;
    s_moveConstructed = 0;
    s_destructed = 0;
    s_copied = 0;
    s_moved = 0;
  }

  static bool check(I32 defaultConstructed, I32 valueConstructed, I32 copyConstructed,
                    I32 moveConstructed, I32 destructed, I32 copied, I32 moved) {
    bool result = s_defaultConstructed == defaultConstructed &&
                  s_valueConstructed == valueConstructed && s_copyConstructed == copyConstructed &&
                  s_moveConstructed == moveConstructed && s_destructed == destructed &&
                  s_copied == copied && s_moved == moved;

    if (!result) {
      LOG(Error) << "s_defaultConstructed = " << s_defaultConstructed
                 << " && s_defaultConstructed = " << defaultConstructed;
      LOG(Error) << "s_valueConstructed = " << s_valueConstructed
                 << " && s_valueConstructed = " << valueConstructed;
      LOG(Error) << "s_copyConstructed = " << s_copyConstructed
                 << " && s_copyConstructed = " << copyConstructed;
      LOG(Error) << "s_moveConstructed = " << s_moveConstructed
                 << " && s_moveConstructed = " << moveConstructed;
      LOG(Error) << "s_destructed = " << s_destructed << " && s_destructed = " << destructed;
      LOG(Error) << "s_copied = " << s_copied << " && s_copied = " << copied;
      LOG(Error) << "s_moved = " << s_moved << " && s_moved = " << moved;
    }

    return result;
  }

  I32 value = 0;

  LifetimeTracker() {
    ++s_defaultConstructed;
  }

  explicit LifetimeTracker(I32 value) : value(value) {
    ++s_valueConstructed;
  }

  LifetimeTracker(const LifetimeTracker& other) : value(other.value) {
    ++s_copyConstructed;
  }

  LifetimeTracker(LifetimeTracker&& other) : value(other.value) {
    other.value = 0;

    ++s_moveConstructed;
  }

  ~LifetimeTracker() {
    ++s_destructed;
  }

  LifetimeTracker& operator=(const LifetimeTracker& other) {
    value = other.value;

    ++s_copied;

    return *this;
  }

  LifetimeTracker& operator=(LifetimeTracker&& other) {
    value = other.value;
    other.value = 0;

    ++s_moved;

    return *this;
  }
};

// static
I32 LifetimeTracker::s_defaultConstructed = 0;
I32 LifetimeTracker::s_valueConstructed = 0;
I32 LifetimeTracker::s_copyConstructed = 0;
I32 LifetimeTracker::s_moveConstructed = 0;
I32 LifetimeTracker::s_destructed = 0;
I32 LifetimeTracker::s_copied = 0;
I32 LifetimeTracker::s_moved = 0;

void takesLifetimeTrackerByValue(LifetimeTracker x) {}

TEST_CASE("callback can hold object as bound argument") {
  LifetimeTracker::reset();
  {
    LifetimeTracker x;
    bind(&takesLifetimeTrackerByValue, x);
  }
  CHECK(LifetimeTracker::check(1, 0, 0, 0, 1, 0, 0));
}

TEST_CASE("copying a callback doesn't copy it's bound arguments") {
  LifetimeTracker::reset();
  {
    const LifetimeTracker x;
    auto cb = bind(&takesLifetimeTrackerByValue, x);
    auto cb2 = cb;
  }
  CHECK(LifetimeTracker::check(1, 0, 0, 0, 1, 0, 0));
}

TEST_CASE("moving a callback doesn't change it's bound arguments") {
  LifetimeTracker::reset();
  {
    const LifetimeTracker x;
    auto cb = bind(&takesLifetimeTrackerByValue, x);
    auto cb2 = std::move(cb);
  }
  CHECK(LifetimeTracker::check(1, 0, 0, 0, 1, 0, 0));
}

I32 returnsLifetimeTrackerValue(LifetimeTracker x) {
  return x.value;
}

TEST_CASE("callback can persist the state of a bound argument") {
  LifetimeTracker::reset();
  {
    LifetimeTracker x(10);
    auto cb = bind(&returnsLifetimeTrackerValue, x);
    CHECK(cb.invoke() == 10);
  }
  // Default constructed on creation.
  // Copy constructed when passed into `bind`.
  // Both gets destructed.
  CHECK(LifetimeTracker::check(0, 1, 1, 0, 2, 0, 0));
}

TEST_CASE("arguments can be moved into callbacks") {
  LifetimeTracker::reset();
  {
    LifetimeTracker x(10);
    auto cb = bind(&returnsLifetimeTrackerValue, std::move(x));
    CHECK(cb.invoke() == 10);
  }
  // Default constructed on creation.
  // Copy constructed when passed into `bind`.
  // Copied into function when invoked, because we take the argument by value.
  // Both gets destructed.
  CHECK(LifetimeTracker::check(0, 1, 1, 1, 2, 0, 0));
}

}  // namespace nu
