#include "core/Value.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Event.hpp"
#include "runtime/Locale.hpp"
#include <cjson/cJSON.h>
#include <exception>
#include <filesystem>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <clocale>
#endif
#include "runtime/Application.hpp"
using namespace cube;
using namespace cube::runtime;

Application &Application::getInstance() {
  static Application theApp;
  return theApp;
}

Application::Application() {}

Application::~Application() {}

auto Application::reset() -> void {
  _loggers.clear();
  _asset.reset();
  _locale.reset();
  _configuration.reset();
  _modLoader.reset();
  initialize();
}

auto Application::initialize() -> void {
  _asset.setDomain(_appname, std::filesystem::current_path());
  _locale.addLanguage("en_US", "English (US)");
  _locale.addLanguageSource("en_US", _appname + ":data/locale/en_US.lang");
  _locale.setDefaultLanguage("en_US");
  _locale.setLanguage("en_US");

  auto &config = _configuration.getConfig(getAppName());
  if (!config.getObject().contains("language")) {
    config.asObject()->insert({"language", core::Value{}.setString("en_US")});
  }
  if (!config.getObject().contains("enableMods")) {
    config.asObject()->insert({"enableMods", core::Value{}.setArray()});
  }
  _configuration.saveConfig(getAppName());

  auto buf = _asset.load(_appname, "script/index.mjs");
  if (buf) {
    std::string src{(const char *)buf->getData(), buf->getSize()};
    _script.run(src, _asset.resolve(_appname, "script/index.mjs"));
  }

  _modLoader.scanMods();
  for (auto &mod : config.getField("enableMods").getArray()) {
    _modLoader.enableMod(mod.getString());
  }
  _modLoader.loadEnableMods();
  _locale.setLanguage(config.getField("language").getString());
}

auto Application::run(int argc, char *argv[]) -> int {
  for (int idx = 0; idx < argc; idx++) {
    _args.push_back(argv[idx]);
  }
  initialize();

  _eventbus.publish<PreInitializeEvent>();
  _eventbus.publish<InitializeEvent>();
  _eventbus.publish<PostInitializeEvent>();

  while (true) {
    if (_taskLoop.hasTask()) {
      _taskLoop.nextTask();
      continue;
    }
    if (_script.hasTask()) {
      _script.nextTask();
      continue;
    }
    break;
  }
  _eventbus.publish<QuitEvent>();
  return 0;
}

auto Application::getArgs() const -> const std::vector<const char *> & {
  return _args;
}
auto Application::getAppName() const -> const std::string & { return _appname; }

auto Application::getAppVersion() const -> const core::Version & {
  return _appversion;
}

auto Application::getTaskLoop() -> TaskLoop & { return _taskLoop; }

auto Application::getLogger(const std::string &name) -> Logger & {
  auto &logger = _loggers[name];
  if (!logger) {
    logger = std::make_unique<Logger>(name, &_loggerTarget);
  }
  return *logger;
}

auto Application::getLoggerTarget() -> LoggerTarget & { return _loggerTarget; }

auto Application::getAsset() -> Asset & { return _asset; }

auto Application::getLocale() -> Locale & { return _locale; }

auto Application::getEventBus() -> EventBus & { return _eventbus; }

auto Application::getModLoader() -> ModLoader & { return _modLoader; }

auto Application::getConfiguration() -> Configuration & {
  return _configuration;
}
auto Application::getScript() -> Script & { return _script; }

auto main(int argc, char **argv) -> int {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#else
  setlocale(LC_ALL, "");
#endif
  auto &theApp = Application::getInstance();
  try {
    return theApp.run(argc, argv);
  } catch (std::exception &e) {
    theApp.getLogger("System").error("Uncaugt error: {}", e.what());
  } catch (...) {
    theApp.getLogger("System").error("Uncaught Error: unknown error");
  }
  return -1;
}