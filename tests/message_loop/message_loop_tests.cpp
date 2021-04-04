#include <catch2/catch.hpp>

#include "nucleus/message_loop/message_loop.h"

namespace nu {

class AddOneTask : public Task {
public:
  explicit AddOneTask(U32* value) : value_{value} {}

  void execute() override {
    *value_ += 1;
  }

private:
  U32* value_;
};

TEST_CASE("MessageLoop") {
  MessageLoop ml;

  SECTION("run until idle") {
    // This run loop should not hang!!
    ml.run_until_idle();
  }

  SECTION("run until quit task") {
    ml.post_quit();
    // This run loop should not hang!!
    ml.run();
  }

  SECTION("executes tasks") {
    U32 value = 10;
    ml.post_task(makeScopedPtr<AddOneTask>(&value));
    ml.run_until_idle();
    REQUIRE(value == 11);
  }

  SECTION("executes chain until it quits") {
    class Task2 : public Task {
      NU_DELETE_COPY_AND_MOVE(Task2);

    public:
      explicit Task2(MessageLoop* message_loop, U32* value) : message_loop_{message_loop}, value_{value} {}

      void execute() override {
        *value_ = 2;
        message_loop_->post_quit();
      }

    private:
      MessageLoop* message_loop_;
      U32* value_;
    };

    class Task1 : public Task {
      NU_DELETE_COPY_AND_MOVE(Task1);

    public:
      Task1(MessageLoop* message_loop, U32* value) : message_loop_{message_loop}, value_{value} {}

      void execute() override {
        *value_ = 1;
        message_loop_->post_task(makeScopedPtr<Task2>(message_loop_, value_));
      }

    private:
      MessageLoop* message_loop_;
      U32* value_;
    };

    U32 value = 0;
    ml.post_task(makeScopedPtr<Task1>(&ml, &value));
    ml.run();
    REQUIRE(value == 2);
  }
}

}  // namespace nu
