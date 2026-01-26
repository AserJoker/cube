#ifndef _H_CUBE_RUNTIME_APPLICATION_
#define _H_CUBE_RUNTIME_APPLICATION_
#include "core/Instance.hpp"
#include "core/Version.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Configuration.hpp"
#include "runtime/EventBus.hpp"
#include "runtime/Locale.hpp"
#include "runtime/Logger.hpp"
#include "runtime/LoggerTarget.hpp"
#include "runtime/ModLoader.hpp"
#include "runtime/Script.hpp"
#include "runtime/TaskLoop.hpp"
#include "video/Device.hpp"
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace cube::runtime {
class Application : public core::Instance {
private:
  std::vector<const char *> _args;
  std::string _appname = APP_NAME;
  core::Version _appversion = core::Version::parse(APP_VERSION);

private:
  Asset _asset;
  Configuration _configuration;
  EventBus _eventbus;
  TaskLoop _taskLoop;
  Locale _locale;
  ModLoader _modLoader;
  LoggerTarget _loggerTarget;
  Script _script;
  SDL_Window *_window = nullptr;
  std::unique_ptr<video::Device> _video = nullptr;

  std::unordered_map<std::string, std::unique_ptr<Logger>> _loggers;

private:
  auto reset() -> void;
  auto initialize() -> void;
  auto dispose() -> void;

public:
  static Application &getInstance();
  Application();
  ~Application() override;
  auto run(int argc, char *argv[]) -> int;
  auto getArgs() const -> const std::vector<const char *> &;
  auto getAppName() const -> const std::string &;
  auto getAppVersion() const -> const core::Version &;

public:
  auto getTaskLoop() -> TaskLoop &;
  auto getLogger(const std::string &name) -> Logger &;
  auto getLoggerTarget() -> LoggerTarget &;
  auto getAsset() -> Asset &;
  auto getLocale() -> Locale &;
  auto getEventBus() -> EventBus &;
  auto getModLoader() -> ModLoader &;
  auto getConfiguration() -> Configuration &;
  auto getScript() -> Script &;
  auto getVideo() -> video::Device &;
};
} // namespace cube::runtime
#endif