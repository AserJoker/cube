#ifndef _H_CUBE_RUNTIME_CONFIGURATION_
#define _H_CUBE_RUNTIME_CONFIGURATION_
#include "core/Instance.hpp"
#include "core/Value.hpp"
#include <string>
#include <unordered_map>
namespace cube::runtime {
class Configuration : public core::Instance {
private:
  std::unordered_map<std::string, core::Value> _configs;

public:
  auto reset() -> void;
  auto getConfig(const std::string &name) -> core::Value &;
  auto saveConfig(const std::string &name) -> void;
};
} // namespace cube::runtime
#endif