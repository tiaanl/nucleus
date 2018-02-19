
#ifndef NUCLEUS_MESSAGE_LOOP_TASK_H_
#define NUCLEUS_MESSAGE_LOOP_TASK_H_

namespace nu {

class Task {
public:
  virtual ~Task() = default;

  virtual void run() = 0;
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_TASK_H_
