#pragma once
#include "IWindow.hpp"
#include "core/EventBus.hpp"
#include "core/ILogger.hpp"
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace cube::runtime {
class Application : public core::Object {
private:
  std::vector<std::string> _argv;
  bool _running = true;
  core::ILogger *_logger = core::Singleton<core::ILogger, false>::get();
  core::EventBus *_eventBus = core::Singleton<core::EventBus>::get();
  IWindow *_window = nullptr;

private:
  static Application *_instance;

private:
  void onWindowClose();
  void runTick();
  void processEvent();

public:
  Application(int argc, char *argv[]);
  ~Application() override;
  int32_t run();
  const std::vector<std::string> &getArgv() const;
  void exit();
  IWindow *getWindow() { return _window; }
  IWindow *createOpenGLWindow(int32_t major, int32_t minor);

public:
  static Application *getInstance() { return Application::_instance; }
  static void *operator new(size_t size) noexcept = delete;
};
} // namespace cube::runtime
