#include "core/Value.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Event.hpp"
#include "runtime/Locale.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <cjson/cJSON.h>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
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

Application::~Application() { dispose(); }

auto Application::reset() -> void {
  _asset.reset();
  _locale.reset();
  _configuration.reset();
  _modLoader.reset();
  initialize();
}

auto Application::initialize() -> void {
  _asset.setDomain(_appname, std::filesystem::current_path().string());
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
  if (!config.getObject().contains("logger")) {
    config.asObject()->insert({"logger", core::Value{}.setString("info")});
  }
  _configuration.saveConfig(getAppName());
  auto logger = config.getField("logger").getString();
  if (logger == "debug") {
    _mask = Logger::Level::DEBUG;
  } else if (logger == "info") {
    _mask = Logger::Level::INFO;
  } else if (logger == "warn") {
    _mask = Logger::Level::WARN;
  } else if (logger == "error") {
    _mask = Logger::Level::ERR;
  } else {
    getLogger("System").warn("Unknown logger mask '{}', use INFO as default",
                             logger);
    _mask = Logger::Level::INFO;
  }
  for (auto &[_, logger] : _loggers) {
    logger->setMask(_mask);
  }
  _locale.setLanguage(config.getField("language").getString());
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
}

auto Application::dispose() -> void {
  _video.reset();
  SDL_DestroyWindow(_window);
  _window = nullptr;
  SDL_Quit();
  SDL_SetLogOutputFunction(nullptr, nullptr);
}

auto Application::run(int argc, char *argv[]) -> int {
  SDL_SetLogOutputFunction(Logger::SDL_LogCallback, &std::cout);
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
  SDL_SetAppMetadata(_appname.c_str(), _appversion.toString().c_str(), nullptr);
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    throw std::runtime_error(std::string("Failed to initialize SDL: ") +
                             SDL_GetError());
  }
  _window = SDL_CreateWindow(_appname.c_str(), 1024, 768, 0);
  if (!_window) {
    throw std::runtime_error(std::string("Failed to create window: ") +
                             SDL_GetError());
  }
  _video = std::make_unique<video::Device>(_window);
  initialize();
  _eventbus.publish<PreInitializeEvent>();
  _eventbus.publish<InitializeEvent>();
  _eventbus.publish<PostInitializeEvent>();

  while (true) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        break;
      }
    } else {
      if (_script.hasTask()) {
        _script.nextTask();
      }
    }
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

auto Application::getAsset() -> Asset & { return _asset; }

auto Application::getLocale() -> Locale & { return _locale; }

auto Application::getEventBus() -> EventBus & { return _eventbus; }

auto Application::getModLoader() -> ModLoader & { return _modLoader; }

auto Application::getConfiguration() -> Configuration & {
  return _configuration;
}
auto Application::getScript() -> Script & { return _script; }

auto Application::getVideo() -> video::Device & { return *_video; }

auto Application::getLogger(const std::string &name) -> Logger & {
  if (!_loggers.contains(name)) {
    _loggers[name] = std::make_unique<Logger>(name);
    _loggers.at(name)->setMask(_mask);
  }
  return *_loggers.at(name);
}

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
    theApp.getLogger("System").error("Uncaught Exception: {}", e.what());
  } catch (...) {
    theApp.getLogger("System").error("Uncaught Exception: unknown exception");
  }
  return -1;
}