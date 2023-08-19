#pragma once

#include "logger.hpp"
#include "time.hpp"
#include "types.hpp"

#include <cstdint>
#include <functional>
#include <queue>
#include <unordered_map>

namespace hyped::core {

class Scheduler {
 public:
  Scheduler(core::ILogger &logger, core::ITimeSource &time);
  /**
   * @brief Calls the task at the top of the queue if current time is greater than the delay
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
   * @param delay the minimum delay in microseconds before the task can called
   */
  void addTask(uint32_t delay, std::function<core::Result(void)> task);

 private:
  core::ILogger &logger_;
  core::ITimeSource &time_;
  std::priority_queue<std::uint64_t, std::vector<std::uint64_t>, std::greater<std::uint64_t>>
    task_queue_;
  std::unordered_map<std::uint64_t, std::function<core::Result(void)>> task_map_;
};

}  // namespace hyped::core