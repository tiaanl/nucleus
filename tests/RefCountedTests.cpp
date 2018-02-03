
#include "nucleus/Allocators/Allocated.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Ref.h"
#include "nucleus/RefCounted.h"
#include "nucleus/Testing.h"

#include "nucleus/MemoryDebug.h"

namespace {

class SelfAssign : public nu::RefCounted<SelfAssign> {
public:
  ~SelfAssign() {}
};

class CheckDerivedMemberAccess : public nu::Ref<SelfAssign> {
public:
  CheckDerivedMemberAccess() {
    // This shouldn't compile if we don't have access to the member variable.
    SelfAssign** pptr = &m_ptr;
    CHECK(m_ptr == *pptr);
  }
};

class RefToSelf : public nu::RefCounted<RefToSelf> {
public:
  RefToSelf() : m_selfPtr(this) {}
  ~RefToSelf() = default;

private:
  nu::Ref<RefToSelf> m_selfPtr;
};

}  // namespace

TEST_CASE("TestSelfAssignment") {
  SelfAssign sa;
  SelfAssign* p = &sa;

  nu::Ref<SelfAssign> var(p);
  var = var;
  CHECK(p == var.get());
}

TEST_CASE("MemberAccess") {
  CheckDerivedMemberAccess check;
}

TEST_CASE("BooleanOperations") {
  SelfAssign s1;

  SelfAssign* p1 = &s1;
  nu::Ref<SelfAssign> p2;

  CHECK(p1);
  CHECK_FALSE(!p1);

  CHECK(!p2);
  CHECK_FALSE(p2);

  CHECK(p1 != p2);

  SelfAssign s2;
  SelfAssign* raw = &s2;

  p2 = raw;
  CHECK(p1 != p2);
  CHECK(p2 == raw);

  p2 = p1;
  CHECK(raw != p2);
  CHECK(p2 == p1);
}

struct RefCountedWithTraits;

struct TestTraits {
  static bool s_destructred;

  static void destruct(const RefCountedWithTraits* o) {
    s_destructred = true;
  }
};

// static
bool TestTraits::s_destructred = false;

struct RefCountedWithTraits : nu::RefCounted<RefCountedWithTraits, TestTraits> {};

TEST_CASE("CallsDestruct") {
  {
    RefCountedWithTraits r;
    nu::Ref<RefCountedWithTraits> p{&r}; }
  CHECK(TestTraits::s_destructred);
}
