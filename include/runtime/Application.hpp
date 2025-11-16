#pragma once
#include "Locale.hpp"
#include "core/Object.hpp"
#include "runtime/Config.hpp"
#include "runtime/Loader.hpp"
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
  std::unique_ptr<Loader> _loader = std::make_unique<Loader>();
  std::unique_ptr<Config> _config = std::make_unique<Config>();
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
  auto getLoader() -> Loader &;
  auto getLoader() const -> const Loader &;
  auto getArguments() const -> const std::vector<std::string> &;
};
} // namespace cube::runtime