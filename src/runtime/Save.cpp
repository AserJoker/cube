#include "runtime/Save.hpp"
#include "runtime/Application.hpp"
#include <cjson/cJSON.h>
using namespace cube;
using namespace cube::runtime;
auto Save::create(const std::string &name) -> std::shared_ptr<Save> {
  auto &app = Application::getInstance();
  auto &loader = app.getLoader();
  auto fullname =
      app.getApplicationName() + ":saves/" + name + "/manifest.json";
  auto save = loader.loadAs<Save>(fullname);
  if (!save) {
    save = std::make_shared<Save>(name);
  }
  return save;
}
Save::Save(const std::shared_ptr<core::Buffer> &buffer) : Object() {
  if (!buffer) {
    return;
  }
  std::string content(static_cast<const char *>(buffer->getData()),
                      buffer->getSize());
  auto node = cJSON_Parse(content.c_str());
  if (!node) {
    return;
  }
  auto value = core::Value::parseJSON(node);
  cJSON_Delete(node);
  if (value.getType() != core::Value::Type::Object) {
    return;
  }
  auto obj = value.asObject();
  if (obj->contains("name")) {
    auto nameVal = obj->at("name").asString();
    if (nameVal) {
      _name = nameVal.value();
    }
  }
  if (obj->contains("version")) {
    auto versionVal = obj->at("version").asString();
    if (versionVal) {
      _version = versionVal.value();
    }
  }
  if (obj->contains("mods")) {
    auto modsVal = obj->at("mods").asArray();
    if (modsVal) {
      for (const auto &modVal : modsVal.value()) {
        auto modStr = modVal.asString();
        if (modStr) {
          _mods.push_back(modStr.value());
        }
      }
    }
  }
  if (obj->contains("data")) {
    auto dataVal = obj->at("data").asObject();
    if (dataVal) {
      for (const auto &[key, val] : dataVal.value()) {
        auto dataStr = val.asString();
        if (dataStr) {
          _datas[key] = dataStr.value();
        }
      }
    }
  }
  if (obj->contains("config")) {
    _config = obj->at("config");
  }
}

Save::Save(const std::string &name) : Object(), _name(name) {
  auto &app = Application::getInstance();
  _version = app.getApplicationVersion();
  _config.setObject();
}