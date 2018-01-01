
#include "gtest/gtest.h"
#include "nucleus/Allocators/DefaultAllocator.h"
#include "nucleus/Memory/Allocated.h"
#include "nucleus/Ref.h"
#include "nucleus/RefCounted.h"

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
    EXPECT_EQ(*pptr, m_ptr);
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

TEST(RefCountedTests, TestSelfAssignment) {
  SelfAssign sa;
  SelfAssign* p = &sa;

  nu::Ref<SelfAssign> var(p);
  var = var;
  EXPECT_EQ(var.get(), p);
}

TEST(RefCountedTests, MemberAccess) {
  CheckDerivedMemberAccess check;
}

TEST(RefCountedTests, BooleanOperations) {
  SelfAssign* p1 = &(SelfAssign{});
  nu::Ref<SelfAssign> p2;

  EXPECT_TRUE(p1);
  EXPECT_FALSE(!p1);

  EXPECT_TRUE(!p2);
  EXPECT_FALSE(p2);

  EXPECT_NE(p1, p2);

  SelfAssign* raw = &(SelfAssign{});

  p2 = raw;
  EXPECT_NE(p1, p2);
  EXPECT_EQ(raw, p2);

  p2 = p1;
  EXPECT_NE(raw, p2);
  EXPECT_EQ(p1, p2);
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

TEST(RefCountedTests, CallsDestruct) {
  { nu::Ref<RefCountedWithTraits> p{&(RefCountedWithTraits{})}; }
  EXPECT_TRUE(TestTraits::s_destructred);
}
