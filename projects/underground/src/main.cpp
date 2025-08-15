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
#include "runtime/EventKeyDown.hpp"
#include "runtime/EventKeyUp.hpp"
#include "runtime/EventMouseButtonDown.hpp"
#include "runtime/EventMouseMove.hpp"
#include "runtime/EventMouseWheel.hpp"
#include "runtime/EventUninitialize.hpp"
#include "runtime/EventUpdate.hpp"
#include "runtime/EventWindowClose.hpp"
#include "runtime/Locale.hpp"
#include <clocale>
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

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

  glm::vec3 _speed{0, 0, 0};

  std::unordered_map<uint32_t, bool> _keymaps;

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
  }

  void onUpdate(runtime::EventUpdate &event) {
    if (_keymaps.contains('a')) {
      _speed.x = -0.5;
    } else if (_keymaps.contains('d')) {
      _speed.x = 0.5;
    } else {
      _speed.x = 0;
    }
    if (_keymaps.contains('w')) {
      _speed.z = 0.5;
    } else if (_keymaps.contains('s')) {
      _speed.z = -0.5;
    } else {
      _speed.z = 0;
    }
    auto timespeed = (event.getTick() / 1000.0f);
    static float roll = 0.0f;
    if (_keymaps.contains('e')) {
      if (roll < 45.0f) {
        roll += 22.5f * timespeed;
        _camera->roll(22.5f * timespeed);
      }
    } else if (_keymaps.contains('q')) {
      if (roll > -45.0f) {
        roll -= 22.5f * timespeed;
        _camera->roll(-22.5f * timespeed);
      }
    } else {
      roll = 0;
      _camera->setUp({0, 1, 0});
    }
    _camera->move(_speed * timespeed);
    _renderer->clear();
    _renderer->draw(_camera, _mesh);
    _window->present();
  }

  void onUninitialize(runtime::EventUninitialize &) {}

  void onWindowClose(runtime::EventWindowClose &event) {}

  void onKeyDown(runtime::EventKeyDown &event) {
    if (event.getCode() == 0x1b) {
      _window->uncaptureMouse();
    }
    _keymaps[event.getCode()] = true;
  }

  void onKeyUp(runtime::EventKeyUp &event) { _keymaps.erase(event.getCode()); }

  void onMouseMove(runtime::EventMouseMove &event) {
    if (!_window->isCaptureMouse()) {
      return;
    }
    auto &mot = event.getMotion();
    float yaw = mot.x * 0.05;
    _camera->yaw(-yaw);
    float pitch = mot.y * 0.05;
    if (pitch < -90) {
      pitch = -90;
    }
    if (pitch > 90) {
      pitch = 90;
    }
    _camera->pitch(-pitch);
    if (!_keymaps.contains('e') && !_keymaps.contains('q')) {
      _camera->setUp({0, 1, 0});
    }
  }
  void onMouseButtonDown(runtime::EventMouseButtonDown &event) {
    if (!_window->isCaptureMouse()) {
      _window->captureMouse();
      return;
    }
  }
  void onMouseWheel(runtime::EventMouseWheel &event) {
    _camera->zoom(event.getDelta().y * -0.5);
  }

  Underground() {
    _asset = core::Singleton<runtime::AssetManager>::get();
    _config = core::Singleton<runtime::ConfigManager>::get();
    _locale = core::Singleton<runtime::Locale>::get();
    _eventBus->on(this, &Underground::onInitialize);
    _eventBus->on(this, &Underground::onUpdate);
    _eventBus->on(this, &Underground::onUninitialize);
    _eventBus->on(this, &Underground::onWindowClose);
    _eventBus->on(this, &Underground::onMouseMove);
    _eventBus->on(this, &Underground::onMouseButtonDown);
    _eventBus->on(this, &Underground::onKeyDown);
    _eventBus->on(this, &Underground::onKeyUp);
    _eventBus->on(this, &Underground::onMouseWheel);
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