#ifndef _H_CUBE_RUNTIME_TASK_LOOP_
#define _H_CUBE_RUNTIME_TASK_LOOP_
#include "Task.hpp"
#include "core/Instance.hpp"
#include <functional>
#include <queue>

namespace cube::runtime {
class TaskLoop : public core::Instance {
private:
  std::queue<Task> _queue;

public:
  auto nextTask() -> void;
  auto hasTask() -> bool;
  auto emit(const Task &task) -> void;
  auto emit(const std::function<void(Task&)> &task) -> void;
};
} // namespace cube::runtime
#endif