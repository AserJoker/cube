#pragma once
#include "reflection/Variable.hpp"
namespace cube::serialization {
class Json {
public:
  static reflection::Variable *parse(const std::string &source);
  static std::string stringify(reflection::Variable *variable);
};
}; // namespace cube::serialization