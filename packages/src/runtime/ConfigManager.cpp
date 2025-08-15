#include "runtime/ConfigManager.hpp"
#include "core/Object.hpp"
#include "reflection/Variable.hpp"
#include "serialization/json.hpp"
#include <SDL3/SDL_iostream.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::filesystem;
namespace cube::runtime {
static void
resolveConfig(core::Object *parent,
              std::unordered_map<std::string, reflection::Variable *> &configs,
              std::vector<std::string> parts, const path &current) {
  if (is_directory(current)) {
    parts.push_back(current.filename().string());
    for (auto &item : directory_iterator(current)) {
      resolveConfig(parent, configs, parts, item);
    }
  } else {
    size_t size = 0;
    void *data = SDL_LoadFile(current.string().c_str(), &size);
    if (!data) {
      return;
    }
    std::string str((const char *)data, size);
    SDL_free(data);
    auto variable = serialization::Json::parse(str.c_str());
    std::string name = "";
    for (auto &item : parts) {
      name += item + ".";
    }
    auto filename = current.filename().string();
    if (current.has_extension()) {
      filename = filename.substr(0, filename.size() -
                                        current.extension().string().size());
    }
    name += filename;
    configs[name] = variable;
    variable->setParent(parent);
  }
}

void ConfigManager::loadConfig(const std::string &path) {
  if (is_directory(path)) {
    for (auto &item : directory_iterator(path)) {
      resolveConfig(this, _configs, {}, item);
    }
  }
}
reflection::Variable *ConfigManager::pollConfig(const std::string &name) {
  if (_configs.contains(name)) {
    auto res = _configs.at(name);
    _configs.erase(name);
    return res;
  }
  return nullptr;
}

reflection::Variable *ConfigManager::getConfig(const std::string &name) {
  if (_configs.contains(name)) {
    return _configs.at(name);
  }
  return nullptr;
}
}; // namespace cube::runtime