#include "runtime/Application.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <locale.h>
#endif

using namespace cube;
using namespace cube::runtime;

auto Application::setApplicationInfo(const std::string &appname,
                                     const std::string &appversion) -> void {
  this->_appname = appname;
  auto version = core::Version::parse(appversion);
  if (!version) {
    _logger->error(
        "Failed to set application version '{}', invalid version format",
        appversion);
  }
  this->_version = version.value();
}

auto Application::getName() const -> const std::string & { return _appname; }

auto Application::getVersion() const -> const core::Version & {
  return _version;
}

auto Application::getInstance() -> Application & {
  static Application instance;
  return instance;
}
auto Application::resolveConfig() -> void {
  auto cfg = _config->load(_appname, "options.json");
  if (cfg.getType() != core::Value::Type::Object) {
    cfg.setObject();
  }
  auto enableMods = cfg.getField("enableMods");
  if (!enableMods || enableMods->getType() != core::Value::Type::Array) {
    cfg.setField("enableMods", core::Value::createArray());
  } else {
    auto resolved = core::Value::createArray();
    auto mods = enableMods->asArray().value();
    for (auto &mod : mods) {
      if (mod.getType() == core::Value::Type::String) {
        auto modstr = mod.asString().value();
        try {
          _modLoader->enableMod(modstr);
          resolved.appendElement(core::Value::createString(modstr));
        } catch (std::exception &e) {
          _logger->error("Failed to enable mod '{}': {}", modstr, e.what());
        }
      }
    }
    cfg.setField("enableMods", resolved);
  }
  try {
    _modLoader->loadAllMods();
  } catch (std::exception &e) {
    _logger->error("{}", e.what());
    cfg.setField("enableMods", core::Value::createArray());
  }
  auto language = cfg.getField("language");
  if (!language || language->getType() != core::Value::Type::String) {
    cfg.setField("language", core::Value::createString("en_US"));
    _locale->setLang("en_US");
  } else {
    if (!_locale->setLang(language->asString().value())) {
      _locale->setLang("en_US");
      cfg.setField("language", core::Value::createString("en_US"));
    }
  }
  _config->save(_appname, "options.json");
}

auto Application::prepareLocale() -> void {
  _locale->addLanguage("en_US", "English (US)");
  _locale->addLanguageSource("en_US", _appname + ":locale/en_US.lang");
}

auto Application::run(int argc, char **argv) -> int {
  for (int idx = 0; idx < argc; idx++) {
    _arguments.push_back(std::string(argv[idx]));
  }
  auto cwd = std::filesystem::path(argv[0]).parent_path().string();
  _asset->addDomain(_appname, cwd);
  _modLoader->scanModList();
  prepareLocale();
  resolveConfig();
  _isRunning = true;
  while (_isRunning) {
    exit();
  }
  return _exitCode;
}

auto Application::exit(int exitCode) -> void {
  _isRunning = false;
  _exitCode = exitCode;
}

auto Application::getLocale() -> Locale & { return *_locale; }

auto Application::getAsset() -> Asset & { return *_asset; }

auto Application::getLogger() -> core::Logger & { return *_logger; }

auto Application::getArguments() const -> const std::vector<std::string> & {
  return _arguments;
}

auto main(int argc, char **argv) -> int {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#else
  setlocale(LC_ALL, "");
#endif
  try {
    return Application::getInstance().run(argc, argv);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
  return -1;
}