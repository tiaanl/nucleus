
#include "nucleus/Memory/RefCounted.h"
#include "nucleus/Memory/ScopedRefPtr.h"

#include "gtest/gtest.h"

namespace {

class SelfAssign : public nu::RefCounted<SelfAssign> {
  friend class nu::RefCounted<SelfAssign>;

  ~SelfAssign() {}
};

class CheckDerivedMemberAccess : public nu::ScopedRefPtr<SelfAssign> {
public:
  CheckDerivedMemberAccess() {
    // This shouldn't compile if we don't have access to the member variable.
    SelfAssign** pptr = &m_ptr;
    EXPECT_EQ(*pptr, m_ptr);
  }
};

class ScopedRefPtrToSelf : public nu::RefCounted<ScopedRefPtrToSelf> {
public:
  ScopedRefPtrToSelf() : m_selfPtr(this) {}

  static bool wasDestroyed() { return m_wasDestroyed; }

  void SelfDestruct() { m_selfPtr = nullptr; }

private:
  friend class nu::RefCounted<ScopedRefPtrToSelf>;
  ~ScopedRefPtrToSelf() { m_wasDestroyed = true; }

  static bool m_wasDestroyed;

  nu::ScopedRefPtr<ScopedRefPtrToSelf> m_selfPtr;
};

bool ScopedRefPtrToSelf::m_wasDestroyed = false;

}  // namespace

TEST(RefCountedUnitTest, TestSelfAssignment) {
  SelfAssign* p = new SelfAssign;
  nu::ScopedRefPtr<SelfAssign> var(p);
  var = var;
  EXPECT_EQ(var.get(), p);
}

TEST(RefCountedUnitTest, ScopedRefPtrMemberAccess) {
  CheckDerivedMemberAccess check;
}

TEST(RefCountedUnitTest, ScopedRefPtrToSelf) {
  ScopedRefPtrToSelf* check = new ScopedRefPtrToSelf();
  EXPECT_FALSE(ScopedRefPtrToSelf::wasDestroyed());
  check->SelfDestruct();
  EXPECT_TRUE(ScopedRefPtrToSelf::wasDestroyed());
}

TEST(RefCountedUnitTest, ScopedRefPtrBooleanOperations) {
  nu::ScopedRefPtr<SelfAssign> p1 = new SelfAssign;
  nu::ScopedRefPtr<SelfAssign> p2;

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
