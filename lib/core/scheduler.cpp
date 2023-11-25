#include "scheduler.hpp"
#include "wall_clock.hpp"

namespace hyped::core {

Scheduler::Scheduler(core::ILogger &logger, core::ITimeSource &time)
    : logger_(logger),
      time_(time),
      task_queue_()
{
}

core::Result Scheduler::run()
{
  if (task_queue_.empty()) { return core::Result::kSuccess; }
  const auto current_time = std::chrono::time_point_cast<std::chrono::nanoseconds>(time_.now());
  auto next_task          = task_queue_.top();
  core::Result result     = core::Result::kSuccess;
  for (size_t i = 0; i < task_queue_.size(); i++) {
    if (current_time < next_task.sheduled_time) { return result; }
    core::Result next_result = next_task.handler();
    if (next_result == core::Result::kFailure) { result = core::Result::kFailure; }
    task_queue_.pop();
    if (task_queue_.empty()) { break; }
    next_task = task_queue_.top();
  }
  return result;
}

void Scheduler::addTask(const core::Duration delay, std::function<core::Result(void)> handler)
{
  const auto execution_timepoint
    = std::chrono::time_point_cast<std::chrono::nanoseconds>(time_.now())
      + std::chrono::nanoseconds(delay);
  const Task task{execution_timepoint, handler};
  task_queue_.push(task);
}

}  // namespace hyped::core