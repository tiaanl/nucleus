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

#include <gtest/gtest.h>

#include "nucleus/message_loop/message_loop.h"
#include "nucleus/message_loop/message_pump_default.h"

namespace nu {

class Foo {
public:
  Foo() {}
  ~Foo() {}

  void test0() {
    ++m_testCount;
  }

  void test1ConstRef(const std::string& a) {
    ++m_testCount;
    m_result.append(a);
  }

  void test1Ptr(std::string* a) {
    ++m_testCount;
    m_result.append(*a);
  }

  void test1Int(int a) {
    m_testCount += a;
  }

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


#if 0
void RunTest_PostDelayedTask_Basic(MessagePumpFactory factory) {
  scoped_ptr<MessagePump> pump(factory());
  MessageLoop loop(pump.Pass());

  // Test that PostDelayedTask results in a delayed task.

  const TimeDelta kDelay = TimeDelta::FromMilliseconds(100);

  int num_tasks = 1;
  Time run_time;

  loop.PostDelayedTask(
      FROM_HERE, Bind(&RecordRunTimeFunc, &run_time, &num_tasks),
      kDelay);

  Time time_before_run = Time::Now();
  loop.Run();
  Time time_after_run = Time::Now();

  EXPECT_EQ(0, num_tasks);
  EXPECT_LT(kDelay, time_after_run - time_before_run);
}
#endif  // 0

}  // namespace nu
