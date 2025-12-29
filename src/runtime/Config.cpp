#include "runtime/Config.hpp"
#include "core/Json.hpp"
#include "core/Value.hpp"
#include "runtime/Application.hpp"
#include <cstring>
#include <cjson/cJSON.h>
#include <memory>
#include <string>

using namespace cube;
using namespace cube::runtime;

auto Config::load(const std::string &domain, const std::string &filename)
    -> core::Value {
  auto &app = Application::getInstance();
  auto fullname = app.getName() + ":config/" + domain + "/" + filename;
  if (_configs.contains(fullname)) {
    return _configs.at(fullname);
  }
  auto &asset = app.getAsset();
  auto value = asset.loadAs<core::Json>(fullname);
  if (!value) {
    _configs[fullname] = core::Value::createObject();
  } else {
    _configs[fullname] = value->value;
  }
  return _configs[fullname];
}

auto Config::save(const std::string &domain, const std::string &filename) const
    -> bool {
  auto &app = Application::getInstance();
  auto fullname = app.getName() + ":config/" + domain + "/" + filename;
  if (!_configs.contains(fullname)) {
    return false;
  }
  auto &value = _configs.at(fullname);
  auto &asset = app.getAsset();
  auto node = core::Value::serializeJSON(value);
  char *content = cJSON_Print(node);
  cJSON_Delete(node);
  auto buffer = std::make_shared<core::Buffer>(strlen(content), content);
  free(content);
  return asset.save(fullname, buffer);
}