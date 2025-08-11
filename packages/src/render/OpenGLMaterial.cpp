#include "render/OpenGLMaterial.hpp"
namespace cube::render {
OpenGLMaterial::OpenGLMaterial() {}
OpenGLMaterial::~OpenGLMaterial() {};
const std::string &OpenGLMaterial::getShader() const { return _shader; };
const std::unordered_map<std::string, std::string> &
OpenGLMaterial::getTextures() const {
  return _textures;
};
void OpenGLMaterial::setTexture(const std::string &slot,
                                const std::string &name) {
  _textures[slot] = name;
};
void OpenGLMaterial::setShader(const std::string &name) { _shader = name; };
void OpenGLMaterial::removeTexture(const std::string &slot) {
  if (_textures.contains(slot)) {
    _textures.erase(slot);
  }
};
bool OpenGLMaterial::hasTexture(const std::string &slot) const {
  return _textures.contains(slot);
};
const std::string &OpenGLMaterial::getTexture(const std::string &slot) const {
  return _textures.at(slot);
};
}; // namespace cube::render