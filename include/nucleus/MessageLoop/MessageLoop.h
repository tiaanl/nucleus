
#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <queue>

#include "nucleus/Memory/Ptr.h"
#include "nucleus/MessageLoop/MessagePump.h"
#include "nucleus/MessageLoop/Task.h"

namespace nu {

class MessageLoop : public MessagePump::Delegate {
public:
  // A `MessageLoop` has a specific type which indicates the set of asynchronous events it may process in addition to
  // tasks.
  //
  //   Default - only supports tasks.
  //   UI      - supports native UI events (e.g. windows messages).
  //   IO      - supports asynchronous IO.
  //   Custom  - when a `MessagePump` was passed to us.
  //
  enum class Type { Default, UI, IO, Custom };

  // Returns the `MessageLoop` object for the current thread, or `nullptr` if none exists.
  static MessageLoop* current();

  // Creates the default `MessagePump` based on the given `type`.  The called owns the returned `MessagePump`.
  static Ptr<MessagePump> createMessagePumpForType(Type type);

  // Constructs a `MessageLoop` of the specified `type`.
  explicit MessageLoop(Type type = Type::Default);

  // Constructs a `MessageLoop` using the given `MessagePump`.
  explicit MessageLoop(Ptr<MessagePump>&& messagePump);

  // Destroy the `MessageLoop` and delete all the tasks still enqueued.
  virtual ~MessageLoop();

  // Returns the type passed to the constructor.
  Type getType() const {
    return m_type;
  }

  // Returns `true` if the `MessageLoop` is currently running.
  // NOTE: May only be called from the thread that invoked the `run` function.
  bool isRunning() const;

  // Invoke the task asynchronously from within a `MessageLoop` some time in the future.  The `MessageLoop` takes
  // ownership of the `Task` and destroyes it after it has been invoked.
  // NOTE: This function may be called from any thread.  The `task` will be invoked on the thread that executes
  // `MessageLoop::run()`.
  void postTask(Ptr<Task> task);

  // Run the message loop and only returns when the `MessageLoop` is quit.
  void run();

  // Process all enqueued tasks, window messages, etc., but don't wait/sleep.  Return as soon as all tasks that can be
  // run are finished.
  void runUntilIdle();

  // Signals the `run` function to return when it becomes idle.  It will continue to process tasks as long as they are
  // enqueued.
  // NOTE: May only be called from the thread that invoked the `run` function.
  void quitWhenIdle();

protected:
  Ptr<MessagePump> m_messagePump;

private:
  // Override: MessagePump::Delegate
  bool doWork() override;
  bool doIdleWork() override;

  void init();
  void reloadTaskQueue();

  const Type m_type;

  // This will be set to true while the `MessageLoop` is running.
  bool m_isRunning = false;

  // If this is set to true, then the message loop will return on the next idle.
  bool m_quitWhenIdle = false;

  std::queue<Ptr<Task>> m_incomingTaskQueue;
  std::queue<Ptr<Task>> m_taskQueue;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_
