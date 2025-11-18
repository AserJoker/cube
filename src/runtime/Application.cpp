#include "runtime/Application.hpp"
#include "core/Error.hpp"
#include "core/Version.hpp"
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

auto Application::getApplicationName() const -> const std::string & {
  return _appname;
}

auto Application::getApplicationVersion() const -> const core::Version & {
  return _version;
}

auto Application::getInstance() -> Application & {
  static Application instance;
  return instance;
}

auto Application::run(int argc, char **argv) -> int {
  for (int idx = 0; idx < argc; idx++) {
    _arguments.push_back(std::string(argv[idx]));
  }
  _asset->addDomain(_appname,
                    std::filesystem::path(argv[0]).parent_path().string());
  _locale->addLanguage("en_US", "English (US)");
  _locale->addLanguageSource("en_US", _appname + ":locale/en_US.lang");
  _modLoader->scanModList();
  _modLoader->enableMod("i18n");
  _modLoader->loadAllMods();
  _locale->setLang("zh_CN");
  auto str = _locale->i18n("system.application.title");
  _logger->info("{} started", str);
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
  } catch (core::Error &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << "at" << std::endl;
    e.printTrace();
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
  }
  return -1;
}