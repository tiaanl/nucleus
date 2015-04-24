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

TEST(MessageLoopTest, PostDelayedTask_InDelayOrder) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that two tasks with different delays run in the right order.
  int numTasks = 2;
  Time runTime1, runTime2;

  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime1, &numTasks),
      std::chrono::milliseconds{200});

  // If we get a large pause in execution (due to a context switch) here, this
  // test could fail.
  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime2, &numTasks),
      std::chrono::milliseconds{10});

  loop.run();

  EXPECT_EQ(0, numTasks);
  EXPECT_TRUE(runTime2 < runTime1);
}

TEST(MessageLoopTest, PostDelayedTask_InPostOrder) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that two tasks with the same delay run in the order in which they were
  // posted.
  const std::chrono::milliseconds kDelay{100};

  int numTasks = 2;
  Time runTime1, runTime2;

  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime1, &numTasks), kDelay);
  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime2, &numTasks), kDelay);

  loop.run();

  EXPECT_EQ(0, numTasks);
  EXPECT_TRUE(runTime1 < runTime2);
}

TEST(MessageLoopTest, PostDelayedTask_InPostOrder_2) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that a delayed task still runs after normal tasks even if the normal
  // tasks take a long time to run.

  const std::chrono::milliseconds kPause{50};

  int numTasks = 2;
  Time runTime;

  loop.postTask(std::bind(&slowFunc, &loop, kPause, &numTasks));
  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime, &numTasks),
      std::chrono::milliseconds{10});

  Time timeBeforeRun = std::chrono::high_resolution_clock::now();
  loop.run();
  Time timeAfterRun = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(0, numTasks);
  EXPECT_LT(kPause, timeAfterRun - timeBeforeRun);
}

TEST(MessageLoopTest, PostDelayedTask_InPostOrder_3) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that a delayed task still runs after a pile of normal tasks.  The key
  // difference between this test and the previous one is that here we return
  // the MessageLoop a lot so we give the MessageLoop plenty of opportunities to
  // maybe run the delayed task.  It should know not to do so until the delayed
  // task's delay has passed.

  int numTasks = 11;
  Time runTime1, runTime2;

  // Clutter the MessageLoop with tasks.
  for (int i = 1; i < numTasks; ++i) {
    loop.postTask(std::bind(&recordRunTimeFunc, &loop, &runTime1, &numTasks));
  }

  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime2, &numTasks),
      std::chrono::milliseconds{1});

  loop.run();

  EXPECT_EQ(0, numTasks);
  EXPECT_TRUE(runTime2 > runTime1);
}

TEST(MessageLoopTest, PostDelayedTask_SharedTimer) {
  using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
  MessageLoop loop(std::make_unique<MessagePumpDefault>());

  // Test that the interval of the timer, used to run the next delayed task, is
  // set to a value corresponding to when the next delayed task should run.

  // By setting numTasks to 1, we ensure that the first task to run causes the
  // run loop to exit.

  int numTasks = 1;
  Time runTime1, runTime2;

  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime1, &numTasks),
      std::chrono::milliseconds{1000});
  loop.postDelayedTask(
      std::bind(&recordRunTimeFunc, &loop, &runTime2, &numTasks),
      std::chrono::milliseconds{10});

  Time startTime = std::chrono::high_resolution_clock::now();

  loop.run();

  EXPECT_EQ(0, numTasks);

  // Ensure that we ran in far less time than the slower timer.
  std::chrono::milliseconds totalTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now() - startTime);
  EXPECT_GT(5000, totalTime.count());

  // In case both timers somehow run at nearly the same time, sleep a little and
  // then run all pending to force them both to have run.
  std::this_thread::sleep_for(std::chrono::milliseconds{100});

  loop.runUntilIdle();

  EXPECT_TRUE(runTime1 == Time());
  EXPECT_FALSE(runTime2 == Time());
}

}  // namespace nu
