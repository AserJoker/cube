#include "runtime/Save.hpp"
#include "core/Buffer.hpp"
#include "core/Error.hpp"
#include "core/Json.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include "runtime/Application.hpp"
#include <cjson/cJSON.h>
#include <memory>
#include <optional>
#include <sstream>
using namespace cube;
using namespace cube::runtime;
auto Save::create(const std::string &name) -> std::shared_ptr<Save> {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto fullname =
      app.getApplicationName() + ":saves/" + name + "/manifest.json";
  auto save = asset.loadAs<Save>(fullname);
  if (!save) {
    save = std::make_shared<Save>(name);
  }
  return save;
}
Save::Save(const std::shared_ptr<core::Buffer> &buffer,
           const std::string &filename) {
  if (!buffer) {
    return;
  }
  auto value = core::Json(buffer, filename).value;
  if (value.getType() != core::Value::Type::Object) {
    throw core::Error(
        "Invalid load manifest {} : invalid format, manifest must be object",
        filename);
  }
  auto obj = value.asObject();
  if (obj->contains("name") ||
      obj->at("name").getType() != core::Value::Type::String) {
    throw core::Error(
        "Invalid load manifest {} : invalid format, field 'name' is required",
        filename);
  }
  _name = obj->at("name").asString().value();
  if (!obj->contains("version") ||
      obj->at("version").getType() != core::Value::Type::String) {

    throw core::Error("Invalid load manifest {} : invalid format, field "
                      "'version' is required",
                      filename);
  }
  auto versionVal = obj->at("version").asString();
  _version = core::Version::parse(versionVal.value()).value();
  if (obj->contains("mods")) {
    auto modsVal = obj->at("mods").asArray();
    if (modsVal) {
      for (const auto &modVal : modsVal.value()) {
        auto modStr = modVal.asString();
        if (modStr) {
          _mods.push_back(modStr.value());
        } else {
          throw core::Error("Invalid load manifest {} : invalid format, field "
                            "'mods' must be string array",
                            filename);
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

Save::~Save() { saveManifest(); }

auto Save::getName() const -> const std::string & { return _name; }
auto Save::getVersion() const -> const core::Version & { return _version; }
auto Save::getMods() const -> const std::vector<std::string> & { return _mods; }
auto Save::getConfig() const -> const core::Value & { return _config; }
auto Save::saveManifest() const -> bool {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  std::stringstream builder;
  builder << app.getApplicationName() << ":saves/" << _name << "/manifest.json";
  auto fullname = builder.str();
  core::Value manifest = core::Value::createObject();
  manifest.setField("name", core::Value::createString(_name));
  manifest.setField(
      "version", core::Value::createString(core::Version::serialize(_version)));
  core::Value mods = core::Value::createArray();
  for (auto &mod : _mods) {
    mods.appendElement(core::Value::createString(mod));
  }
  manifest.setField("mods", mods);
  manifest.setField("config", _config);
  cJSON *node = core::Value::serializeJSON(manifest);
  char *string = cJSON_Print(node);
  auto buf = std::make_shared<core::Buffer>(strlen(string), string);
  cJSON_free(string);
  return asset.save(fullname, buf);
}
auto Save::save(const std::string &filename,
                const std::shared_ptr<core::Buffer> &buffer) -> bool {
  if (filename == "manifest.json") {
    return false;
  }
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  std::stringstream builder;
  builder << app.getApplicationName() << ":saves/" << _name << "/" << filename;
  return asset.save(builder.str(), buffer);
}
auto Save::load(const std::string &filename) -> std::shared_ptr<core::Buffer> {
  if (filename == "manifest.json") {
    return nullptr;
  }
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  std::stringstream builder;
  builder << app.getApplicationName() << ":saves/" << _name << "/" << filename;
  return asset.load(builder.str());
}