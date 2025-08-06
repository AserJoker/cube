#pragma once
#include "core/Object.hpp"
#include <string>
namespace cube::render {
class IMaterial : public core::Object {
private:
  std::string _shader = "cube.shader.default";

public:
  inline const std::string &getShader() const { return _shader; };
};
}; // namespace cube::render