#include "runtime/Configuration.hpp"
#include "core/Buffer.hpp"
#include "core/Value.hpp"
#include "runtime/Application.hpp"
#include <memory>
using namespace cube;
using namespace cube::runtime;

auto Configuration::reset() -> void { _configs.clear(); }

auto Configuration::getConfig(const std::string &name) -> core::Value & {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto &logger = app.getLogger("Configuration");
  if (!_configs.contains(name)) {
    auto buf =
        asset.load(app.getAppName(), std::format("config/{}.json", name));
    if (!buf) {
      logger.warn("Failed to load config: {}", name);
      _configs[name].setObject();
    } else {
      std::string src = {(const char *)buf->getData(), buf->getSize()};
      auto json = core::Value::fromJSON(src);
      if (json) {
        if (json->getType() != core::Value::Type::OBJECT) {
          json->setObject();
        }
        _configs[name] = *json;
      } else {
        logger.warn("Failed to load config: {} invalid format", name);
        _configs[name].setObject();
      }
    }
  }
  return _configs.at(name);
}
auto Configuration::saveConfig(const std::string &name) -> void {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto buf = std::make_shared<core::Buffer>();
  if (_configs.contains(name)) {
    std::string json = _configs[name].toJSON();
    buf->setData(json.length(), json.data());
  }
  asset.save(app.getAppName(), std::format("config/{}.json", name), buf);
}