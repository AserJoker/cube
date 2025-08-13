#pragma once
#include "render/IShader.hpp"
namespace cube::render {
class Shader : public IShader {
private:
  std::string _name;

public:
  Shader(const std::string &name);
  const std::string &getName() const override;
};
} // namespace cube::render