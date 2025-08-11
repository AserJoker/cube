#pragma once
#include "IMaterial.hpp"
namespace cube::render {
class OpenGLMaterial : public IMaterial {

private:
  std::string _shader = "cube.shader.basic";
  std::unordered_map<std::string, std::string> _textures;

public:
  OpenGLMaterial();
  ~OpenGLMaterial() override;
  const std::string &getShader() const override;
  const std::unordered_map<std::string, std::string> &
  getTextures() const override;
  void setTexture(const std::string &slot, const std::string &name) override;
  void setShader(const std::string &name) override;
  void removeTexture(const std::string &slot) override;
  bool hasTexture(const std::string &slot) const override;
  const std::string &getTexture(const std::string &idx) const override;
};
} // namespace cube::render