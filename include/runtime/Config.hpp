#pragma once
#include "core/Object.hpp"
#include "core/Value.hpp"
#include <string>
#include <unordered_map>
namespace cube::runtime {
class Config : public core::Object {
private:
  std::unordered_map<std::string, core::Value> _configs;

public:
  auto load(const std::string &domain, const std::string &filename)
      -> core::Value;
  auto save(const std::string &domain, const std::string &filename) const
      -> bool;
};
} // namespace cube::runtime