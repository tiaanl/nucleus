// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include "nucleus/message_loop/message_loop.h"
#include "nucleus/message_loop/message_pump_default.h"

namespace nu {

namespace {

class Foo {
public:
  Foo() {}
  ~Foo() {}

  void test0() { ++m_testCount; }

  void test1ConstRef(const std::string& a) {
    ++m_testCount;
    m_result.append(a);
  }

  void test1Ptr(std::string* a) {
    ++m_testCount;
    m_result.append(*a);
  }

  void test1Int(int a) { m_testCount += a; }

  void test2Ptr(std::string* a, std::string* b) {
    ++m_testCount;
    m_result.append(*a);
    m_result.append(*b);
  }

  void test2Mixed(const std::string& a, std::string* b) {
    ++m_testCount;
    m_result.append(a);
    m_result.append(*b);
  }

  int getTestCount() const { return m_testCount; }

  const std::string& getResult() const { return m_result; }

private:
  int m_testCount = 0;
  std::string m_result;

  DISALLOW_COPY_AND_ASSIGN(Foo);
};

// This function runs slowly to simulate a large amount of work being done.
void slowFunc(MessageLoop* messageLoop, const std::chrono::milliseconds& delay,
              int* quitCounter) {
  std::this_thread::sleep_for(delay);
  if (--(*quitCounter) == 0) {
    messageLoop->quitWhenIdle();
  }
}

// This function records the time when run was called, which is useful for
// building a variety of MessageLoop tests.
void recordRunTimeFunc(
    MessageLoop* messageLoop,
    std::chrono::time_point<std::chrono::high_resolution_clock>* runTime,
    int* quitCounter) {
  *runTime = std::chrono::high_resolution_clock::now();

  // Cause our run function to take some time to execute.  As a result we can
  // count on subsequent recordRunTimeFunc() running at a future time wihout
  // worry about the resolution of our system clock being an issue.
  slowFunc(messageLoop, std::chrono::milliseconds(100), quitCounter);
}

}  // namespace

TEST(MessageLoopTest, PostTask) {
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Add tests to message loop.
  auto foo = std::make_shared<Foo>();
  std::string a("a"), b("b"), c("c"), d("d");

  loop.postTask(std::bind(&Foo::test0, foo));
  loop.postTask(std::bind(&Foo::test1ConstRef, foo, a));
  loop.postTask(std::bind(&Foo::test1Ptr, foo, &b));
  loop.postTask(std::bind(&Foo::test1Int, foo, 100));
  loop.postTask(std::bind(&Foo::test2Ptr, foo, &a, &c));
  loop.postTask(std::bind(&Foo::test2Mixed, foo, a, &d));

  // After all tests, post a message that will shut down the message loop.
  loop.postTask(std::bind(&MessageLoop::quitWhenIdle, &loop));

  // Now run the loop.
  loop.run();

  EXPECT_EQ(foo->getTestCount(), 105);
  EXPECT_EQ(foo->getResult(), "abacad");
}

TEST(MessageLoopTest, PostDelayedTask_Basic) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that postDelayedTask results in a delayed task.
  const std::chrono::milliseconds kDelay{100};

  int numTasks = 1;
  Time runTime;

  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime, &numTasks), kDelay);

  Time timeBeforeRun = std::chrono::high_resolution_clock::now();
  loop.run();
  Time timeAfterRun = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(0, numTasks);
  EXPECT_LT(kDelay, timeAfterRun - timeBeforeRun);
}

}  // namespace nu
