#include "runtime/Task.hpp"
#include <chrono>
#include <cstdint>
#include <thread>
using namespace cube;
using namespace cube::runtime;
using namespace std::chrono;

auto Task::run(Task &task,
               const std::chrono::system_clock::time_point &now) const -> bool {
  if (!_exec) {
    return false;
  }
  if (_delay && now - _start < _delay * 1ms) {
    std::this_thread::sleep_for(4ms);
    return true;
  }
  _exec(task);
  return _keep;
}
auto Task::setKeep(bool keep) -> void { _keep = keep; }
auto Task::cancel() -> void { _exec = nullptr; }
Task::Task(const std::function<void(Task &)> &exec, bool keep, uint64_t delay)
    : _exec(exec), _keep(keep), _start(system_clock::now()), _delay(delay) {}