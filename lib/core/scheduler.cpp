#include "scheduler.hpp"
#include "wall_clock.hpp"

namespace hyped::core {

Scheduler::Scheduler(core::ILogger &logger, core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      task_queue_(),
      task_map_()
{
}

core::Result Scheduler::run()
{
  if (task_queue_.empty()) { return core::Result::kSuccess; }
  const auto current_time = std::chrono::time_point_cast<std::chrono::microseconds>(time_.now())
                              .time_since_epoch()
                              .count();
  const auto next_task_time = task_queue_.top();
  if (current_time < next_task_time) { return core::Result::kSuccess; }
  task_queue_.pop();
  const auto task = task_map_.find(next_task_time);
  if (task == task_map_.end()) {
    logger_.log(core::LogLevel::kFatal, "Scheduled task missing callback function");
    return core::Result::kFailure;
  }
  task_map_.erase(next_task_time);
  const auto result = task->second();
  return result;
}

void Scheduler::addTask(std::function<core::Result(void)> task, uint32_t delay)
{
  const auto execution_timepoint
    = std::chrono::time_point_cast<std::chrono::microseconds>(time_.now())
      + std::chrono::microseconds(delay);
  std::uint64_t execution_timepoint_us = execution_timepoint.time_since_epoch().count();
  // If the timepoint is already in the map, increment it until it is not
  while (task_map_.find(execution_timepoint_us) != task_map_.end()) {
    execution_timepoint_us++;
  }
  task_queue_.push(execution_timepoint_us);
  task_map_.emplace(execution_timepoint_us, task);
}

}  // namespace hyped::core