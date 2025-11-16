#include "runtime/Application.hpp"
#include <filesystem>
#include <string>
using namespace cube;
using namespace cube::runtime;

auto Application::setApplicationInfo(const std::string &appname,
                                     const std::string &appversion) -> void {
  this->_appname = appname;
  this->_version = appversion;
}

auto Application::getApplicationName() const -> const std::string & {
  return _appname;
}

auto Application::getApplicationVersion() const -> const std::string & {
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
  _loader->addDomain(_appname,
                     std::filesystem::path(argv[0]).parent_path().string());
  _locale->addLanguage("en_US", "English (US)");
  _locale->addLanguageSource("en_US", _appname + ":locales/en_US.lang");
  _locale->setLang("en_US");
  auto cfg = _config->load("cube", "engine.json");
  _config->save("cube", "demo.json", cfg);
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

auto Application::getLocale() const -> const Locale & { return *_locale; }

auto Application::getLoader() -> Loader & { return *_loader; }

auto Application::getLoader() const -> const Loader & { return *_loader; }

auto Application::getArguments() const -> const std::vector<std::string> & {
  return _arguments;
}

auto main(int argc, char **argv) -> int {
  return Application::getInstance().run(argc, argv);
}