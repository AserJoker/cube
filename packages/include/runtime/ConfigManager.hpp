#pragma once
#include "core/Object.hpp"
#include "reflection/Variable.hpp"
#include <string>
#include <unordered_map>
namespace cube::runtime {
class ConfigManager : public core::Object {
private:
  std::unordered_map<std::string, reflection::Variable *> _configs;

public:
  void loadConfig(const std::string &path);
  reflection::Variable *pollConfig(const std::string &name);
  reflection::Variable *getConfig(const std::string &name);
};
} // namespace cube::runtime