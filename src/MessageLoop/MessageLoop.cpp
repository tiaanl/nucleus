
#include "nucleus/MessageLoop/MessageLoop.h"
#include "nucleus/Logging.h"
#include "nucleus/MessageLoop/MessagePumpDefault.h"

namespace nu {

namespace {

thread_local MessageLoop* g_messageLoopOnThisThread = nullptr;

}  // namespace

// static
MessageLoop* MessageLoop::current() {
  return g_messageLoopOnThisThread;
}

MessageLoop::MessageLoop() {
  init();

  m_messagePump.reset(new MessagePumpDefault);
}

MessageLoop::MessageLoop(Ptr<MessagePump>&& messagePump) : m_messagePump{std::move(messagePump)} {
  DCHECK(m_messagePump.get());

  init();
}

MessageLoop::~MessageLoop() {
  DCHECK(this == current());

  // Clean up the message loop on this thread.
  g_messageLoopOnThisThread = nullptr;
}

bool MessageLoop::isRunning() const {
  DCHECK(this == current());

  return m_isRunning;
}

void MessageLoop::postTask(Ptr<Task> task) {
  m_incomingTaskQueue.push(std::move(task));
}

void MessageLoop::run() {
  m_isRunning = true;
  m_messagePump->run(this);
  m_isRunning = false;
}

void MessageLoop::quitWhenIdle() {
  m_quitWhenIdle = true;
}

bool MessageLoop::doWork() {
  for (;;) {
    // Load incoming work into the task queue.
    reloadTaskQueue();
    if (m_taskQueue.empty()) {
      break;
    }

    // Run all the tasks we currently have in the queue.
    do {
      Ptr<Task> task = std::move(m_taskQueue.front());
      m_taskQueue.pop();

      task->run();

    } while (!m_taskQueue.empty());
  }

  // Nothing happened.
  return false;
}

void MessageLoop::runUntilIdle() {
  m_quitWhenIdle = true;
  run();
}

bool MessageLoop::doIdleWork() {
  if (m_quitWhenIdle) {
    m_messagePump->quit();
  }

  return false;
}

void MessageLoop::init() {
  DCHECK(!current()) << "Should only have one message loop per thread.";

  // This `MessageLoop` is not the owner of this thread.
  g_messageLoopOnThisThread = this;
}

void MessageLoop::reloadTaskQueue() {
  if (m_taskQueue.empty()) {
    std::swap(m_taskQueue, m_incomingTaskQueue);
  }
}

}  // namespace nu
