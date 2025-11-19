#pragma once
#include "Locale.hpp"
#include "Window.hpp"
#include "core/Logger.hpp"
#include "core/Object.hpp"
#include "core/Version.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Config.hpp"
#include "runtime/ModLoader.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace cube::runtime {
class Application : public core::Object {
private:
  std::string _appname = APP_NAME;
  core::Version _version =
      core::Version::parse(APP_VERSION).value_or({0, 1, 0});

private:
  bool _isRunning = false;
  int _exitCode = 0;
  std::unique_ptr<Locale> _locale = std::make_unique<Locale>();
  std::unique_ptr<Asset> _asset = std::make_unique<Asset>();
  std::unique_ptr<Config> _config = std::make_unique<Config>();
  std::unique_ptr<core::Logger> _logger = std::make_unique<core::Logger>();
  std::unique_ptr<ModLoader> _modLoader = std::make_unique<ModLoader>();
  std::vector<std::string> _arguments;
  std::unordered_map<SDL_WindowID, std::unique_ptr<Window>> _windows;
  SDL_WindowID _mainWindowID;
  SDL_WindowID _activeWindowID;

private:
  Application() = default;
  ~Application() override = default;

public:
  static auto getInstance() -> Application &;
  static auto logHook(core::Logger *logger, int category,
                      SDL_LogPriority priority, const char *message) -> void;

private:
  auto resolveConfig() -> void;
  auto prepareLocale() -> void;
  auto createMainWindow() -> void;
  auto onMainLoop() -> void;

public:
  auto setInfo(const std::string &appname, const std::string &appversion)
      -> void;
  auto getName() const -> const std::string &;
  auto getVersion() const -> const core::Version &;
  auto run(int argc, char **argv) -> int;
  auto exit(int exitCode = 0) -> void;
  auto getLocale() -> Locale &;
  auto getAsset() -> Asset &;
  auto getLogger() -> core::Logger &;
  auto getArguments() const -> const std::vector<std::string> &;
};
} // namespace cube::runtime