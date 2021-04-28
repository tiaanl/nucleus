#pragma once

#include "nucleus/Macros.h"
#include "nucleus/Types.h"
#include "nucleus/hash.h"

namespace nu {

namespace testing {

class LifetimeTracker {
public:
  static I32 creates;
  static I32 destroys;
  static I32 copies;
  static I32 moves;

  static void reset() {
    LifetimeTracker::creates = 0;
    LifetimeTracker::destroys = 0;
    LifetimeTracker::copies = 0;
    LifetimeTracker::moves = 0;
  }

  LifetimeTracker() : a_{0}, b_{0} {
    creates++;
  }

  LifetimeTracker(I32 a, I32 b) : a_(a), b_(b) {
    creates++;
  }

  LifetimeTracker(const LifetimeTracker& other) : a_(other.a_), b_(other.b_) {
    copies++;
  }

  LifetimeTracker(LifetimeTracker&& other) noexcept : a_(other.a_), b_(other.b_) {
    other.a_ = 0;
    other.b_ = 0;

    moves++;
  }

  ~LifetimeTracker() {
    destroys++;
  }

  LifetimeTracker& operator=(const LifetimeTracker& other) {
    a_ = other.a_;
    b_ = other.b_;

    copies++;

    return *this;
  }

  LifetimeTracker& operator=(LifetimeTracker&& other) {
    a_ = other.a_;
    b_ = other.b_;

    other.a_ = 0;
    other.b_ = 0;

    moves++;

    return *this;
  }

  auto operator==(const LifetimeTracker& other) const -> bool {
    return a_ == other.a_ && b_ == other.b_;
  }

  auto operator!=(const LifetimeTracker& other) const -> bool {
    return !operator==(other);
  }

  NU_NO_DISCARD I32 a() const {
    return a_;
  }

  NU_NO_DISCARD I32 b() const {
    return b_;
  }

private:
  I32 a_;
  I32 b_;
};

}  // namespace testing

template <>
struct Hash<testing::LifetimeTracker> {
  static HashedValue hashed(const testing::LifetimeTracker& value) {
    return hash_qword(value.a() + value.b());
  }
};

}  // namespace nu
