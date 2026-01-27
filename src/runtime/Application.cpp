#include "core/Value.hpp"
#include "runtime/Asset.hpp"
#include "runtime/Event.hpp"
#include "runtime/Locale.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <cjson/cJSON.h>
#include <exception>
#include <filesystem>
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
  _loggers.clear();
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
  _locale.setLanguage(config.getField("language").getString());
  auto logger = config.getField("logger").getString();
  if (logger == "debug") {
    Logger::setMask(Logger::Level::DEBUG);
  } else if (logger == "info") {
    Logger::setMask(Logger::Level::INFO);
  } else if (logger == "log") {
    Logger::setMask(Logger::Level::LOG);
  } else if (logger == "warn") {
    Logger::setMask(Logger::Level::WARN);
  } else if (logger == "error") {
    Logger::setMask(Logger::Level::ERR);
  } else {
    getLogger("System").warn("Unknown logger mask '{}', use info as default",
                             logger);
    Logger::setMask(Logger::Level::LOG);
  }
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

static void SDL_Logger_Callback(void *userdata, int category,
                                SDL_LogPriority priority, const char *message) {
  std::string cate = "Custom";
  switch (category) {
  case SDL_LOG_CATEGORY_APPLICATION:
    cate = "Application";
    break;
  case SDL_LOG_CATEGORY_ERROR:
    cate = "Error";
    break;
  case SDL_LOG_CATEGORY_ASSERT:
    cate = "Assert";
    break;
  case SDL_LOG_CATEGORY_SYSTEM:
    cate = "System";
    break;
  case SDL_LOG_CATEGORY_AUDIO:
    cate = "Audio";
    break;
  case SDL_LOG_CATEGORY_VIDEO:
    cate = "Video";
    break;
  case SDL_LOG_CATEGORY_RENDER:
    cate = "Render";
    break;
  case SDL_LOG_CATEGORY_INPUT:
    cate = "Input";
    break;
  case SDL_LOG_CATEGORY_TEST:
    cate = "Test";
    break;
  case SDL_LOG_CATEGORY_GPU:
    cate = "GPU";
    break;
  }
  auto &logger = Application::getInstance().getLogger("SDL:" + cate);
  switch (priority) {
  case SDL_LOG_PRIORITY_INVALID:
  case SDL_LOG_PRIORITY_TRACE:
    logger.error("{}", message);
    break;
  case SDL_LOG_PRIORITY_VERBOSE:
  case SDL_LOG_PRIORITY_DEBUG:
    logger.debug("{}", message);
    break;
  case SDL_LOG_PRIORITY_INFO:
    logger.info("{}", message);
    break;
  case SDL_LOG_PRIORITY_WARN:
    logger.warn("{}", message);
    break;
  case SDL_LOG_PRIORITY_ERROR:
  case SDL_LOG_PRIORITY_CRITICAL:
    logger.error("{}", message);
    break;
  case SDL_LOG_PRIORITY_COUNT:
    logger.debug("{}", message);
    break;
  }
}

auto Application::run(int argc, char *argv[]) -> int {
  for (int idx = 0; idx < argc; idx++) {
    _args.push_back(argv[idx]);
  }
  SDL_SetLogOutputFunction(SDL_Logger_Callback, nullptr);
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
  SDL_SetAppMetadata(_appname.c_str(), _appversion.toString().c_str(), nullptr);
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    throw std::runtime_error(std::string("Failed to initialize SDL: ") +
                             SDL_GetError());
  }
  _taskLoop.emit([](Task &task) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        task.cancel();
      }
    }
    task.setKeep(true);
  });

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
    if (_taskLoop.hasTask() || _script.hasTask()) {
      if (_taskLoop.hasTask()) {
        _taskLoop.nextTask();
      }
      if (_script.hasTask()) {
        _script.nextTask();
      }
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

auto Application::getVideo() -> video::Device & { return *_video; }

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