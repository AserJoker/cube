#pragma once
#include "RenderMode.hpp"
#include "core/Object.hpp"
#include <string>
#include <unordered_map>
namespace cube::render {
class Material : public core::Object {

private:
  std::string _shader = "cube.shader.basic";
  std::unordered_map<std::string, std::string> _textures;
  bool _depthTest = false;
  bool _stencilTest = false;
  bool _alphaTest = false;
  bool _transparent = false;
  float _opacity = 1.0f;
  bool _blend = false;
  RenderMode _mode = RenderMode::TRIANGLES;

public:
  Material();
  ~Material() override;
  const std::string &getShader() const;
  const std::unordered_map<std::string, std::string> &getTextures() const;
  Material *setTexture(const std::string &slot, const std::string &name);
  Material *setShader(const std::string &name);
  Material *removeTexture(const std::string &slot);
  bool hasTexture(const std::string &slot) const;
  const std::string &getTexture(const std::string &idx) const;
  bool isDepthTest();
  Material *enableDepthTest();
  Material *disableDepthTest();
  bool isStencilTest() const;
  Material *enableStencilTest();
  Material *disableStencilTest();
  bool isAlphaTest() const;
  Material *enableAlphaTest();
  Material *disableAlphaTest();
  bool isTransparent() const;
  Material *enableTransparent();
  Material *disableTransparent();
  Material *setOpacity(float value);
  float getOpacity() const;
  bool isBlend() const;
  Material *enableBlend();
  Material *disableBlend();
  const RenderMode& getRenderMode() const;
  Material * setRenderMode(const RenderMode& mode);

};
} // namespace cube::render