
#include <catch2/catch.hpp>

#include "nucleus/memory/scoped_ref_ptr.h"
#include "nucleus/ref_counted.h"

namespace {

class SelfAssign : public nu::RefCounted<SelfAssign> {
public:
  ~SelfAssign() {}
};

class CheckDerivedMemberAccess : public nu::ScopedRefPtr<SelfAssign> {
public:
  CheckDerivedMemberAccess() {
    // This shouldn't compile if we don't have access to the member variable.
    SelfAssign** pptr = &ptr_;
    CHECK(ptr_ == *pptr);
  }
};

class RefToSelf : public nu::RefCounted<RefToSelf> {
public:
  RefToSelf() : m_selfPtr(this) {}
  ~RefToSelf() = default;

private:
  nu::ScopedRefPtr<RefToSelf> m_selfPtr;
};

}  // namespace

TEST_CASE("MemberAccess") {
  CheckDerivedMemberAccess check;
}

TEST_CASE("BooleanOperations") {
  SelfAssign* p1 = new SelfAssign;
  nu::ScopedRefPtr<SelfAssign> p2;

  CHECK(p1);
  CHECK_FALSE(!p1);

  CHECK(!p2);
  CHECK_FALSE(p2);

  // CHECK(p1 != p2);

  SelfAssign* s2 = new SelfAssign;
  SelfAssign* raw = s2;

  p2 = raw;
  //CHECK(p1 != p2);
  CHECK(p2 == raw);

  p2 = p1;
  //CHECK(raw != p2);
  CHECK(p2 == p1);
}

struct RefCountedWithTraits;

struct TestTraits {
  static bool s_destructred;

  static void destruct(const RefCountedWithTraits*) {
    s_destructred = true;
  }
};

// static
bool TestTraits::s_destructred = false;

struct RefCountedWithTraits : nu::RefCounted<RefCountedWithTraits, TestTraits> {};

TEST_CASE("CallsDestruct") {
  {
    RefCountedWithTraits r;
    nu::ScopedRefPtr<RefCountedWithTraits> p{&r};
  }
  CHECK(TestTraits::s_destructred);
}
