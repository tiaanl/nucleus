
#include "nucleus/Logging.h"
#include "nucleus/MessageLoop/MessageLoop.h"
#include "nucleus/Testing.h"

namespace nu {

namespace {

I32 g_singleTaskRunCount = 0;

class SingleTask : public Task {
public:
  COPY_DELETE(SingleTask);
  MOVE_DELETE(SingleTask);

  SingleTask(I32 a, I32 b) : m_a{a}, m_b{b} {}

  void run() override {
    g_singleTaskRunCount++;
    LOG(Info) << m_a << " + " << m_b << " = " << m_a + m_b;
  }

private:
  I32 m_a;
  I32 m_b;
};

}  // namespace

TEST_CASE() {
  MessageLoop ml;

  MessageLoop::current()->postTask(makePtr<SingleTask>(3, 4));
  ml.postTask(makePtr<SingleTask>(1, 2));

  ml.runUntilIdle();

  CHECK(g_singleTaskRunCount == 2);
}

}  // namespace nu
