#include "runtime/Config.hpp"
#include "core/Json.hpp"
#include "core/Value.hpp"
#include "runtime/Application.hpp"
#include <cjson/cJSON.h>
#include <memory>
#include <string>

using namespace cube;
using namespace cube::runtime;

auto Config::load(const std::string &domain, const std::string &filename) const
    -> core::Value {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto fullname =
      app.getApplicationName() + ":config/" + domain + "/" + filename;
  return asset.loadAs<core::Json>(fullname)->value;
}

auto Config::save(const std::string &domain, const std::string &filename,
                  const core::Value &value) const -> bool {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto node = core::Value::serializeJSON(value);
  char *content = cJSON_Print(node);
  auto fullname =
      app.getApplicationName() + ":config/" + domain + "/" + filename;
  cJSON_Delete(node);
  auto buffer = std::make_shared<core::Buffer>(std::strlen(content), content);
  free(content);
  return asset.save(fullname, buffer);
}