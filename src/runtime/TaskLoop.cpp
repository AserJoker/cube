#include "runtime/TaskLoop.hpp"
#include "runtime/Application.hpp"
#include <chrono>
#include <exception>
using namespace cube;
using namespace cube::runtime;

auto TaskLoop::nextTask() -> void {
  if (_queue.empty()) {
    return;
  }
  auto now = std::chrono::system_clock::now();
  auto &task = _queue.front();
  auto &theApp = Application::getInstance();
  try {
    if (task.run(task, now)) {
      _queue.push(task);
    }
  } catch (std::exception &e) {
    theApp.getLogger("System").error("Uncaugt error: {}", e.what());
  } catch (...) {
    theApp.getLogger("System").error("Uncaught Error: unknown error");
  }
  _queue.pop();
}
auto TaskLoop::hasTask() -> bool { return !_queue.empty(); }
auto TaskLoop::emit(const Task &task) -> void { _queue.push(task); }
auto TaskLoop::emit(const std::function<void(Task &)> &task) -> void {
  _queue.push({task});
}