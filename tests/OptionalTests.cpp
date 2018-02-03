
#include "nucleus/Optional.h"

#include "nucleus/Testing.h"

#include "nucleus/MemoryDebug.h"

static int constructorCalled = 0;
static int destructorCalled = 0;
static int copyCalled = 0;
static int moveCalled = 0;

struct MockObject {
  int value;

  explicit MockObject(int value) : value(value) {
    constructorCalled++;
  }

  MockObject(const MockObject& other) : value(other.value) {
    copyCalled++;
  }

  MockObject(MockObject&& other) : value(other.value) {
    moveCalled++;
  }

  ~MockObject() {
    destructorCalled++;
  }

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

TEST_CASE("Optional can hold an object or not") {
  constructorCalled = 0;
  destructorCalled = 0;

  nu::Optional<int> i1;
  REQUIRE(!i1.isValid());

  nu::Optional<int> i2(10);
  REQUIRE(i2.isValid());
  REQUIRE(i2.get() == 10);

  i2.reset();
  REQUIRE(!i2.isValid());
}

TEST_CASE("default initialization does not construct the value") {
  constructorCalled = 0;
  destructorCalled = 0;

  nu::Optional<MockObject> empty;
  REQUIRE(!empty.isValid());
  REQUIRE(constructorCalled == 0);
  REQUIRE(destructorCalled == 0);
}

TEST_CASE("we can make an optional with a helper") {
  constructorCalled = 0;
  destructorCalled = 0;

  {
    auto o1 = nu::makeOptional<MockObject>(10);

    // MockObject should only have been constructed once.
    CHECK(constructorCalled == 1);

    // MockObject should have been destructed once when the temporary is destroyed inside MakeOptional.
    CHECK(destructorCalled == 1);

    // MockObject is moved once, when it's moved into the Optional.
    CHECK(moveCalled == 1);
  }

  // Value should never be copied.
  CHECK(copyCalled == 0);

  // MockObject is destructed one more time when the Optional is destructed.
  CHECK(destructorCalled == 2);
}

TEST_CASE("destruct with trivial destructor") {
  // This is an obscure test, but is to make sure we can compile with a type with trivial destructor.
  nu::Optional<int> i = nu::makeOptional<int>(10);
}

TEST_CASE("optional as return value") {
  nu::Optional<MockObject> o1 = getValue(0);
  CHECK(!o1.isValid());

  nu::Optional<MockObject> o2 = getValue(1);
  CHECK(o2.isValid());
  CHECK(o2.get().value == 1);

  // TODO: Make sure the copy/move values are correct when returning an Optional.
}

TEST_CASE("compare Optional with a value") {
  nu::Optional<int> empty;
  nu::Optional<int> one;

  CHECK(empty != 1);
  CHECK(1 != empty);
  CHECK(one != 1);
  CHECK(1 != one);

  CHECK(empty != 1);
  CHECK(1 != empty);
  CHECK(one != 1);
  CHECK(1 != one);
}
