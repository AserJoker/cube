#include "runtime/Application.hpp"
#include "core/Error.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
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
  _asset->addDomain(_appname,
                    std::filesystem::path(argv[0]).parent_path().string());
  _isRunning = true;
  _logger->log("Application start with {}:{}", _appname, _version);

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

auto Application::getAsset() -> Asset & { return *_asset; }

auto Application::getAsset() const -> const Asset & { return *_asset; }

auto Application::getArguments() const -> const std::vector<std::string> & {
  return _arguments;
}

auto main(int argc, char **argv) -> int {
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