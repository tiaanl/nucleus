
#include "nucleus/Ref.h"
#include "nucleus/RefCounted.h"

#include "gtest/gtest.h"

#include "nucleus/MemoryDebug.h"

namespace {

class SelfAssign : public nu::RefCounted {
  friend class nu::RefCounted;

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

class RefToSelf : public nu::RefCounted {
public:
  RefToSelf() : m_selfPtr(this) {}

  static bool wasDestroyed() {
    return m_wasDestroyed;
  }

  void SelfDestruct() {
    m_selfPtr = nullptr;
  }

private:
  friend class nu::RefCounted;

  ~RefToSelf() {
    m_wasDestroyed = true;
  }

  static bool m_wasDestroyed;

  nu::Ref<RefToSelf> m_selfPtr;
};

bool RefToSelf::m_wasDestroyed = false;

}  // namespace

TEST(RefCountedTests, TestSelfAssignment) {
  SelfAssign* p = new SelfAssign;
  nu::Ref<SelfAssign> var(p);
  var = var;
  EXPECT_EQ(var.get(), p);
}

TEST(RefCountedTests, ScopedRefPtrMemberAccess) {
  CheckDerivedMemberAccess check;
}

TEST(RefCountedTests, RefToSelf) {
  RefToSelf* check = new RefToSelf();
  EXPECT_FALSE(RefToSelf::wasDestroyed());
  check->SelfDestruct();
  EXPECT_TRUE(RefToSelf::wasDestroyed());
}

TEST(RefCountedTests, RefBooleanOperations) {
  nu::Ref<SelfAssign> p1 = new SelfAssign;
  nu::Ref<SelfAssign> p2;

  EXPECT_TRUE(p1);
  EXPECT_FALSE(!p1);

  EXPECT_TRUE(!p2);
  EXPECT_FALSE(p2);

  EXPECT_NE(p1, p2);

  SelfAssign* raw_p = new SelfAssign;
  p2 = raw_p;
  EXPECT_NE(p1, p2);
  EXPECT_EQ(raw_p, p2);

  p2 = p1;
  EXPECT_NE(raw_p, p2);
  EXPECT_EQ(p1, p2);
}
