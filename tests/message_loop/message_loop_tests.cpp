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

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include <gtest/gtest.h>

#include "nucleus/message_loop/message_loop.h"
#include "nucleus/logging.h"

namespace nu {

namespace {

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
      messageLoop->postTask(std::bind(&RunCounter::runOnce, this, duration));
    }
  }
};

void longRunningTask(const std::chrono::milliseconds& taskTime) {
  LOG(Info) << "Starting long running task: " << taskTime.count();
  std::this_thread::sleep_for(taskTime);
}

}  // namespace

TEST(MessageLoopTest, DISABLED_Basic) {
  MessageLoop loop;
  RunCounter runCounter;

  // Add some tasks that need to be run.
  loop.postTask(std::bind(&RunCounter::runOnce, std::ref(runCounter),
                          std::chrono::seconds(1)));
  loop.postTask(std::bind(&RunCounter::runOnce, std::ref(runCounter),
                          std::chrono::seconds(1)));
  loop.runUntilIdle();

  EXPECT_EQ(2, runCounter.runCount);
}

TEST(MessageLoopTest, DISABLED_AddTaskFromDifferentThread) {
  // We should be able to add tasks to the message loop while there is a very
  // long task currently running.

  MessageLoop loop;
  RunCounter runCounter;

  std::atomic<size_t> counter{0};

  std::thread t2([&]() {
    // Post a long running task.
    loop.postTask(std::bind(&longRunningTask, std::chrono::seconds(10)));
    ++counter;

    // Sleep for a little bit and then post 2 more tasks.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    loop.postTask(std::bind(&longRunningTask, std::chrono::milliseconds(100)));
    ++counter;
    loop.postTask(std::bind(&longRunningTask, std::chrono::milliseconds(100)));
    ++counter;

    // When this task is run, we should have posted all the previous tasks.
    loop.postTask(std::bind([&]() { EXPECT_EQ(3, counter); }));
  });

  loop.runUntilIdle();

  t2.join();
}

}  // namespace nu
