#include "runtime/Config.hpp"
#include "core/Value.hpp"
#include "runtime/Application.hpp"
#include <cjson/cJSON.h>
#include <filesystem>
#include <memory>
#include <string>

using namespace cube;
using namespace cube::runtime;

auto Config::load(const std::string &domain, const std::string &filename) const
    -> core::Value {
  auto &app = Application::getInstance();
  auto &loader = app.getLoader();
  auto fullname =
      app.getApplicationName() + ":config/" + domain + "/" + filename;
  auto buffer = loader.load(fullname);
  if (!buffer) {
    return core::Value::createNull();
  }
  std::string content(static_cast<const char *>(buffer->getData()),
                      buffer->getSize());
  auto node = cJSON_Parse(content.c_str());
  if (!node) {
    return core::Value::createNull();
  }
  auto value = core::Value::parseJSON(node);
  cJSON_Delete(node);
  return value;
}

auto Config::save(const std::string &domain, const std::string &filename,
                  const core::Value &value) const -> bool {
  auto &app = Application::getInstance();
  auto &loader = app.getLoader();
  auto node = core::Value::serializeJSON(value);
  if (!node) {
    return false;
  }
  char *content = cJSON_Print(node);
  cJSON_Delete(node);
  if (!content) {
    return false;
  }
  auto buffer = std::make_shared<core::Buffer>(std::strlen(content), content);
  free(content);
  auto fullname =
      app.getApplicationName() + ":config/" + domain + "/" + filename;
  auto path = loader.resolvePath(fullname);
  std::filesystem::path dirPath = std::filesystem::path(path).parent_path();
  if (!std::filesystem::exists(dirPath)) {
    std::filesystem::create_directories(dirPath);
  }
  if (path.empty()) {
    return false;
  }
  FILE *file = fopen(path.c_str(), "wb");
  if (!file) {
    return false;
  }
  fwrite(buffer->getData(), 1, buffer->getSize(), file);
  fclose(file);
  return true;
}