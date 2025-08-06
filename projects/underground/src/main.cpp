#include "core/Singleton.hpp"
#include "core/Subscriber.hpp"
#include "render/IRenderer.hpp"
#include "runtime/Application.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/EventInitialize.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include <clocale>
#include <filesystem>

#ifdef __WIN32__
#include <windows.h>
#endif

using namespace cube;

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

uint32_t indices[] = {0, 1, 2};

class Underground : public core::Subscriber {
private:
  runtime::IWindow *_window{};
  runtime::AssetManager *_assetManager =
      core::Singleton<runtime::AssetManager>::get();
  render::IRenderer *_renderer{};

public:
  void onInitialize(runtime::EventInitialize &) {
    auto assetsPath = std::filesystem::current_path().append("assets").string();
    _assetManager->initStore(assetsPath);
    auto application = runtime::Application::getInstance();
    _window = application->createOpenGLWindow(4, 2);
    _renderer = _window->getRenderer();
    _renderer->setClearColor({0.2f, 0.3f, 0.3f, 1.0f});
  }

  void onUpdate(runtime::EventUpdate &) {
    _renderer->clear();
    _window->present();
  }

  void onUninitialize(runtime::EventUninitialize &) {}

  Underground() {
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