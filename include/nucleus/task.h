#pragma once

namespace nu {

class Task {
public:
  virtual ~Task() = default;

  virtual void execute() = 0;
};

}  // namespace nu
