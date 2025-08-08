#include "core/Singleton.hpp"
#include "core/Subscriber.hpp"
#include "render/IRenderer.hpp"
#include "runtime/Application.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/ConfigManager.hpp"
#include "runtime/EventInitialize.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include "runtime/Locale.hpp"
#include <clocale>
#include <filesystem>
#include <print>
#include <string>

#ifdef __WIN32__
#include <windows.h>
#endif

using namespace cube;

class Underground : public core::Subscriber {
private:
  runtime::IWindow *_window{};
  runtime::AssetManager *_assetManager{};
  runtime::ConfigManager *_configManager{};
  runtime::Locale *_locale{};
  render::IRenderer *_renderer{};

public:
  void onInitialize(runtime::EventInitialize &) {
    auto assetsPath = std::filesystem::current_path().append("assets").string();
    _assetManager->initStore(assetsPath);
    auto configPath = std::filesystem::current_path().append("config").string();
    _configManager->loadConfig(configPath);
    auto application = runtime::Application::getInstance();
    _window = application->createOpenGLWindow(4, 2);
    _renderer = _window->getRenderer();
    _renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    _locale->setLanguage("en_US");
    std::println("{}", _locale->i18n("language.name"));
    _locale->setLanguage("zh_CN");
    std::println("{}", _locale->i18n("language.name"));
  }

  void onUpdate(runtime::EventUpdate &) {
    _renderer->clear();
    _window->present();
  }

  void onUninitialize(runtime::EventUninitialize &) {}

  Underground() {
    _assetManager = core::Singleton<runtime::AssetManager>::get();
    _configManager = core::Singleton<runtime::ConfigManager>::get();
    _locale = core::Singleton<runtime::Locale>::get();
    _eventBus->on(this, &Underground::onInitialize);
    _eventBus->on(this, &Underground::onUpdate);
    _eventBus->on(this, &Underground::onUninitialize);
  }
};
auto main(int argc, char *argv[]) -> int {
#ifdef __WIN32__
  SetConsoleOutputCP(CP_UTF8);
#endif
  setlocale(LC_ALL, "");
  runtime::Application theApp(argc, argv);
  theApp.create<Underground>();
  return theApp.run();
}