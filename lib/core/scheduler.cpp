#include "scheduler.hpp"

#include <utility>
namespace hyped::core {

Scheduler::Scheduler(core::ILogger &logger, core::ITimeSource &time) : logger_(logger), time_(time)
{
}

core::Result Scheduler::run()
{
  const auto current_time    = std::chrono::time_point_cast<std::chrono::nanoseconds>(time_.now());
  const auto number_of_tasks = task_queue_.size();
  for (std::size_t i = 0; i < number_of_tasks; i++) {
    const auto next_task = task_queue_.top();
    if (current_time < next_task.sheduled_time) { return core::Result::kSuccess; }
    const auto next_result = next_task.handler();
    task_queue_.pop();
    if (next_result == core::Result::kFailure) { return core::Result::kFailure; }
    if (task_queue_.empty()) { return core::Result::kSuccess; }
  }
  return core::Result::kSuccess;
}

void Scheduler::schedule(const core::Duration delay, std::function<core::Result(void)> handler)
{
  auto execution_timepoint = time_.now() + std::chrono::nanoseconds(delay);
  const Task task(execution_timepoint, std::move(handler));
  task_queue_.push(task);
}

}  // namespace hyped::core
