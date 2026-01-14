#ifndef _H_CUBE_RUNTIME_TASK_
#define _H_CUBE_RUNTIME_TASK_
#include "core/Object.hpp"
#include <chrono>
#include <functional>
namespace cube::runtime {
class Task : public core::Object {
private:
  bool _keep = false;
  std::function<void(Task &)> _exec;
  std::chrono::system_clock::time_point _start;
  uint64_t _delay;

public:
  auto run(Task &task, const std::chrono::system_clock::time_point &now) const
      -> bool;
  auto setKeep(bool keep) -> void;
  auto cancel() -> void;
  Task(const std::function<void(Task &)> &exec, bool keep = false,
       uint64_t delay = 0);
  Task(const Task &) = default;
};
} // namespace cube::runtime
#endif