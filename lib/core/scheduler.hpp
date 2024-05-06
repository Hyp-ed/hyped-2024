#pragma once

#include "logger.hpp"
#include "time.hpp"
#include "types.hpp"

#include <functional>
#include <queue>

namespace hyped::core {

class Scheduler {
  struct Task {
    core::TimePoint sheduled_time;
    std::function<core::Result(void)> handler;
    bool operator>(const Task &other) const { return sheduled_time > other.sheduled_time; }
  };

 public:
  Scheduler(core::ILogger &logger, core::ITimeSource &time);
  /**
   * @brief Runs every task in the queue for which the current time is greater than the time point
   * specified by the task
   *
   * @return kSuccess if no task is called, otherwise returns return value of the task
   */
  core::Result run();
  /**
   * @brief Adds a task to the queue
   * @note The task will be called at minimum after the delay specified by the task
   * @note If two task have the same delay, the task added first will be called first
   *
   * @param task a function that returns a Result
   * @param delay the minimum delay before the task can called
   */
  void schedule(const core::Duration delay, const std::function<core::Result(void)> handler);

 private:
  core::ILogger &logger_;
  core::ITimeSource &time_;
  std::priority_queue<Task, std::vector<Task>, std::greater<>> task_queue_;
};

}  // namespace hyped::core
