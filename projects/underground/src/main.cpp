#include "core/Singleton.hpp"
#include "core/Subscriber.hpp"
#include "render/Camera.hpp"
#include "render/IMesh.hpp"
#include "render/IRenderer.hpp"
#include "render/PerspectiveCamera.hpp"
#include "runtime/Application.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/ConfigManager.hpp"
#include "runtime/EventInitialize.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include "runtime/EventWindowClose.hpp"
#include "runtime/Locale.hpp"
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <clocale>
#include <filesystem>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <string>

#ifdef __WIN32__
#include <windows.h>
#endif

using namespace cube;

class Underground : public core::Subscriber {
private:
  runtime::IWindow *_window{};
  runtime::AssetManager *_asset{};
  runtime::ConfigManager *_config{};
  runtime::Locale *_locale{};
  render::IRenderer *_renderer{};

  render::IMesh *_mesh{};

  render::Camera *_camera{};

public:
  void onInitialize(runtime::EventInitialize &) {
    auto assetsPath = std::filesystem::current_path().append("assets").string();
    _asset->initStore(assetsPath);
    auto configPath = std::filesystem::current_path().append("config").string();
    _config->loadConfig(configPath);
    auto application = runtime::Application::getInstance();
    _window = application->createOpenGLWindow(4, 2);
    _renderer = _window->getRenderer();
    _renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glm::vec3 vertices[] = {
        {0.5f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {-0.5f, -0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
    };
    glm::vec2 texcoords[] = {
        {1.f, 1.f},
        {1.f, 0.f},
        {0.f, 0.f},
        {0.f, 1.f},
    };
    uint32_t indices[] = {0, 1, 3, 1, 2, 3};
    _mesh = _renderer->createMesh();
    auto geometory = _mesh->getGeometory();

    auto attr = geometory->createAttribute(0, false, 3, 0);
    attr->reset(sizeof(vertices), vertices);
    geometory->enableAttribute(0);

    attr = geometory->createAttribute(1, false, 2, 0);
    attr->reset(sizeof(texcoords), texcoords);
    geometory->enableAttribute(1);

    auto index = geometory->createIndexAttribute();
    index->reset(sizeof(indices), indices);

    auto material = _mesh->getMaterial();
    _renderer->loadTexture("cube.texture.sky", "jpg:cube.texture.sky");
    material->setTexture("texture", "cube.texture.sky");

    _camera = new render::PerspectiveCamera(45, 1024, 768, 0.1f, 1000.0f);
    
    auto rot =
        glm::rotate(glm::mat4(1.0f), glm::radians(-55.f), glm::vec3(1.0f, 0, 0));
    _mesh->getGeometory()->applyMatrix(rot);
  }

  void onUpdate(runtime::EventUpdate &) {
    _renderer->clear();
    _renderer->draw(_camera, _mesh);
    _window->present();
  }

  void onUninitialize(runtime::EventUninitialize &) {}

  void onWindowClose(runtime::EventWindowClose &event) {}

  Underground() {
    _asset = core::Singleton<runtime::AssetManager>::get();
    _config = core::Singleton<runtime::ConfigManager>::get();
    _locale = core::Singleton<runtime::Locale>::get();
    _eventBus->on(this, &Underground::onInitialize);
    _eventBus->on(this, &Underground::onUpdate);
    _eventBus->on(this, &Underground::onUninitialize);
    _eventBus->on(this, &Underground::onWindowClose);
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
  return 0;
}