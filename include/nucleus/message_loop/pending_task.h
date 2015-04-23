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

#ifndef NUCLEUS_MESSAGE_LOOP_PENDING_TASK_H_
#define NUCLEUS_MESSAGE_LOOP_PENDING_TASK_H_

#include <cstdint>
#include <chrono>
#include <functional>

namespace nu {

struct PendingTask {
  using Task = std::function<void()>;

  // The task to run.
  Task task;

  // The time when the task was posted.
  std::chrono::time_point<std::chrono::high_resolution_clock> timePosted;

  // The time when the task should be run.
  std::chrono::time_point<std::chrono::high_resolution_clock> delayedRunTime;

  // Secondary sort key for run time.
  int32_t sequenceNum = 0;

  PendingTask(const Task& task)
    : task(task), delayedRunTime(std::chrono::high_resolution_clock::now()) {}

  PendingTask(const Task& task,
              const std::chrono::time_point<std::chrono::high_resolution_clock>&
                  delayedRunTime)
    : task(task), delayedRunTime(delayedRunTime) {}
};

inline bool operator<(const PendingTask& left, const PendingTask& right) {
  // Since the top of the priority queue is defined as the "greatest" element,
  // we need to invert the comparison here.  We want the smaller time to be at
  // the top of the heap.

  if (left.delayedRunTime < right.delayedRunTime)
    return false;

  if (left.delayedRunTime > right.delayedRunTime)
    return true;

  // If the times happen to match, then we use the sequence number to decide.
  // Compare the difference to support integer roll-over.
  return (left.sequenceNum - right.sequenceNum) > 0;
}

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_PENDING_TASK_H_
