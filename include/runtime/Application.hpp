#pragma once
#include "Locale.hpp"
#include "core/Logger.hpp"
#include "core/Object.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Config.hpp"
#include <memory>
#include <string>
#include <vector>

namespace cube::runtime {
class Application : public core::Object {
private:
  std::string _appname = "cube";
  std::string _version = "0.1.0";

private:
  bool _isRunning = false;
  int _exitCode = 0;
  std::unique_ptr<Locale> _locale = std::make_unique<Locale>();
  std::unique_ptr<Asset> _asset = std::make_unique<Asset>();
  std::unique_ptr<Config> _config = std::make_unique<Config>();
  std::unique_ptr<core::Logger> _logger = std::make_unique<core::Logger>();
  std::vector<std::string> _arguments;

private:
  Application() = default;
  ~Application() override = default;

public:
  static auto getInstance() -> Application &;

public:
  auto setApplicationInfo(const std::string &appname,
                          const std::string &appversion) -> void;
  auto getApplicationName() const -> const std::string &;
  auto getApplicationVersion() const -> const std::string &;
  auto run(int argc, char **argv) -> int;
  auto exit(int exitCode = 0) -> void;
  auto getLocale() -> Locale &;
  auto getLocale() const -> const Locale &;
  auto getAsset() -> Asset &;
  auto getAsset() const -> const Asset &;
  auto getArguments() const -> const std::vector<std::string> &;
};
} // namespace cube::runtime