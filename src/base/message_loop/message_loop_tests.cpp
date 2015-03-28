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

#include <chrono>
#include <functional>
#include <thread>

#include "base/message_loop/message_loop.h"
#include "base/logging.h"
#include "gtest/gtest.h"

namespace base {

struct RunCounter {
  int32_t runCount;

  RunCounter() : runCount(0) {}

  void runOnce(const std::chrono::milliseconds& delay) {
    ++runCount;
    LOG(Info) << "Running once - begin";
    std::this_thread::sleep_for(delay);
    LOG(Info) << "Running once - end";
  }

  void addTasksToMessageLoop(MessageLoop* messageLoop, size_t count,
                             const std::chrono::milliseconds& duration) {
    for (auto i = 0; i < count; ++i) {
      LOG(Info) << "Adding task to message loop.";
      messageLoop->addTask(std::bind(&RunCounter::runOnce, this, duration));
    }
  }
};

TEST(MessageLoopTest, Basic) {
  MessageLoop loop;
  RunCounter runCounter;

  // Add some tasks that need to be run.
  loop.addTask(std::bind(&RunCounter::runOnce, std::ref(runCounter),
                         std::chrono::seconds(1)));
  loop.addTask(std::bind(&RunCounter::runOnce, std::ref(runCounter),
                         std::chrono::seconds(1)));
  loop.requestQuit();

  loop.run();

  EXPECT_EQ(2, runCounter.runCount);
}

TEST(MessageLoopTest, AddTaskFromDifferentThread) {
  MessageLoop loop;
  RunCounter runCounter;

  std::thread t(std::bind(&RunCounter::addTasksToMessageLoop,
                          std::ref(runCounter), &loop, 5,
                          std::chrono::seconds(5)));

  loop.run();

  t.join();
}

}  // namespace base
