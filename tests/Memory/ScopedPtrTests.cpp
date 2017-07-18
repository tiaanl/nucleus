
#include "gtest/gtest.h"
#include "nucleus/Memory/ScopedPtr.h"

namespace {

struct CustomDeleter {
    template <typename T>
    void operator()(const T* ptr) const {
        delete const_cast<T*>(ptr);
    }
};

struct CustomArrayDeleter {
    template <typename T>
    void operator()(const T* ptr) const {
        delete[] const_cast<T*>(ptr);
    }
};

struct A {
    char ch;
    static int count;

    A(char c = 0) : ch(c) { ++count; }

    A(const A& other) : ch(other.ch) { ++count; }

    A& operator=(const A& other) {
        ch = other.ch;
        return *this;
    }

    virtual ~A() { --count; }
};

int A::count = 0;

struct B : public A {};

}  // namespace

TEST(ScopedPtrTests, Basic) {
    EXPECT_EQ(0, A::count);

    nu::ScopedPtr<int> t1(new int(5));
    EXPECT_EQ(5, *t1);

    *t1 = 3;
    EXPECT_EQ(3, *t1);

    nu::ScopedPtr<A> t2(new A(1));
    EXPECT_EQ(1, t2->ch);
    EXPECT_EQ(1, A::count);

    nu::ScopedPtr<A> t1b(new B);
    EXPECT_NE(nullptr, t1b.get());
    EXPECT_EQ(2, A::count);

    A* a = t1b.release();
    EXPECT_EQ(nullptr, t1b.get());
    EXPECT_EQ(2, A::count);

    delete a;
    EXPECT_EQ(1, A::count);

    t2->ch = 5;
    EXPECT_EQ(5, t2.get()->ch);

    t2.reset(new A(2));
    EXPECT_EQ(2, t2->ch);
    EXPECT_EQ(1, A::count);

    t2.reset(0);
    EXPECT_EQ(static_cast<A*>(0), t2.get());
    EXPECT_EQ(0, A::count);

    t2.reset(new A(3));
    EXPECT_EQ(3, t2->ch);
    EXPECT_EQ(1, A::count);

    nu::ScopedPtr<A> t3(new A(4));
    EXPECT_EQ(4, t3->ch);
    EXPECT_EQ(2, A::count);

    t2.swap(t3);
    EXPECT_EQ(4, t2->ch);
    EXPECT_EQ(3, t3->ch);
    EXPECT_EQ(2, A::count);

    nu::swap(t2, t3);
    EXPECT_EQ(3, t2->ch);
    EXPECT_EQ(4, t3->ch);
    EXPECT_EQ(t2 < t3, t2.get() < t3.get());
    EXPECT_EQ(2, A::count);

    nu::ScopedPtr<A> releasePtr(new A);
    EXPECT_EQ(3, A::count);
    a = releasePtr.release();
    delete a;
    EXPECT_EQ(2, A::count);

    nu::ScopedPtr<A> t4;
    EXPECT_EQ(nullptr, t4.get());
    if (t4)
        EXPECT_TRUE(t4.get());
    if (!(!t4)) {
        EXPECT_TRUE(t4.get());
    }

    t4.reset(new A(0));
    if (!t4)
        EXPECT_TRUE(!t4.get());

    EXPECT_EQ(3, A::count);

    nu::ScopedPtr<A> t5(nullptr);
    EXPECT_EQ(nullptr, t5.get());

    CustomDeleter customADeleter;
    nu::ScopedPtr<A, CustomDeleter> t6(new A(17), customADeleter);
    EXPECT_EQ(17, t6->ch);

    nu::ScopedPtr<A, CustomDeleter> t7(new A(18), CustomDeleter());
    EXPECT_EQ(18, t7->ch);

    nu::ScopedPtr<A, CustomDeleter> t8(nu::move(t7));
    EXPECT_EQ(18, t8->ch);

    nu::ScopedPtr<A, nu::detail::DefaultDelete<A>> t9(nu::move(t2));

    t6 = nullptr;
    EXPECT_EQ(nullptr, t6.get());
}
