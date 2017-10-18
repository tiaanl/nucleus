
#include "gtest/gtest.h"
#include "nucleus/Optional.h"

static int constructorCalled = 0;
static int destructorCalled = 0;
static int copyCalled = 0;
static int moveCalled = 0;

struct MockObject {
  int value;

  explicit MockObject(int value) : value(value) { constructorCalled++; }

  MockObject(const MockObject& other) : value(other.value) { copyCalled++; }

  MockObject(MockObject&& other) : value(other.value) { moveCalled++; }

  ~MockObject() { destructorCalled++; }

  MockObject& operator=(const MockObject& other) {
    copyCalled++;
    value = other.value;
    return *this;
  }

  MockObject& operator=(MockObject&& other) {
    moveCalled++;
    value = other.value;
    return *this;
  }
};

nu::Optional<MockObject> getValue(int value) {
  return !value ? nu::Optional<MockObject>() : nu::makeOptional<MockObject>(value);
}

TEST(OptionalTests, Basic) {
  constructorCalled = 0;
  destructorCalled = 0;

  nu::Optional<int> i1;
  ASSERT_FALSE(i1.isValid());

  nu::Optional<int> i2(10);
  ASSERT_TRUE(i2.isValid());
  ASSERT_EQ(10, i2.get());

  i2.reset();
  ASSERT_FALSE(i2.isValid());
}

TEST(OptionalTests, DefaultInitializationDoesntConstructValue) {
  constructorCalled = 0;
  destructorCalled = 0;

  nu::Optional<MockObject> empty;
  ASSERT_FALSE(empty.isValid());
  ASSERT_EQ(0, constructorCalled);
  ASSERT_EQ(0, destructorCalled);
}

TEST(OptionalTests, MakeOptional) {
  constructorCalled = 0;
  destructorCalled = 0;

  {
    auto o1 = nu::makeOptional<MockObject>(10);

    // MockObject should only have been constructed once.
    EXPECT_EQ(1, constructorCalled);

    // MockObject should have been destructed once when the temporary is destroyed inside MakeOptional.
    EXPECT_EQ(1, destructorCalled);

    // MockObject is moved once, when it's moved into the Optional.
    EXPECT_EQ(1, moveCalled);
  }

  // Value should never be copied.
  EXPECT_EQ(0, copyCalled);

  // MockObject is destructed one more time when the Optional is destructed.
  EXPECT_EQ(2, destructorCalled);
}

TEST(OptionalTests, DestructWithTrivialDestructor) {
  // This is an obscure test, but is to make sure we can compile with a type with trivial destructor.
  nu::Optional<int> i = nu::makeOptional<int>(10);
}

TEST(OptionalTests, OptionalAsReturnValue) {
  nu::Optional<MockObject> o1 = getValue(0);
  EXPECT_FALSE(o1.isValid());

  nu::Optional<MockObject> o2 = getValue(1);
  EXPECT_TRUE(o2.isValid());
  EXPECT_EQ(1, o2.get().value);

  // TODO: Make sure the copy/move values are correct when returning an Optional.
}

TEST(OptionalTests, CompareOptionalAndValue) {
  nu::Optional<int> empty;
  nu::Optional<int> one;

  EXPECT_FALSE(empty == 1);
  EXPECT_FALSE(1 == empty);
  EXPECT_FALSE(one == 1);
  EXPECT_FALSE(1 == one);

  EXPECT_TRUE(empty != 1);
  EXPECT_TRUE(1 != empty);
  EXPECT_TRUE(one != 1);
  EXPECT_TRUE(1 != one);
}
