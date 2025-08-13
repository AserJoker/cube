#include "render/Material.hpp"
namespace cube::render {
Material::Material() {}
Material::~Material() {};
const std::string &Material::getShader() const { return _shader; };
const std::unordered_map<std::string, std::string> &
Material::getTextures() const {
  return _textures;
};
Material *Material::setTexture(const std::string &slot,
                               const std::string &name) {
  _textures[slot] = name;
  return this;
};
Material *Material::setShader(const std::string &name) {
  _shader = name;
  return this;
};
Material *Material::removeTexture(const std::string &slot) {
  if (_textures.contains(slot)) {
    _textures.erase(slot);
  }
  return this;
};
bool Material::hasTexture(const std::string &slot) const {
  return _textures.contains(slot);
};
const std::string &Material::getTexture(const std::string &slot) const {
  return _textures.at(slot);
};

bool Material::isDepthTest() { return _depthTest; }
Material *Material::enableDepthTest() {
  _depthTest = true;
  return this;
}
Material *Material::disableDepthTest() {
  _depthTest = false;
  return this;
}
bool Material::isStencilTest() const { return _stencilTest; }
Material *Material::enableStencilTest() {
  _stencilTest = true;
  return this;
}
Material *Material::disableStencilTest() {
  _stencilTest = false;
  return this;
}
bool Material::isAlphaTest() const { return _alphaTest; }
Material *Material::enableAlphaTest() {
  _alphaTest = true;
  return this;
}
Material *Material::disableAlphaTest() {
  _alphaTest = false;
  return this;
}
bool Material::isTransparent() const { return _transparent; }
Material *Material::enableTransparent() {
  _transparent = true;
  return this;
}
Material *Material::disableTransparent() {
  _transparent = false;
  return this;
}
Material *Material::setOpacity(float value) {
  _opacity = value;
  return this;
}
float Material::getOpacity() const { return _opacity; }
bool Material::isBlend() const { return _blend; }
Material *Material::enableBlend() {
  _blend = true;
  return this;
}
Material *Material::disableBlend() {
  _blend = false;
  return this;
}
const RenderMode &Material::getRenderMode() const { return _mode; }
Material *Material::setRenderMode(const RenderMode &mode) {
  _mode = mode;
  return this;
}
}; // namespace cube::render