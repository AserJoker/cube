#include "render/OpenGLRenderer.hpp"
#include "render/IMesh.hpp"
#include "render/IShader.hpp"
#include "render/ITexture.hpp"
#include "render/OpenGLGeometory.hpp"
#include "render/OpenGLMesh.hpp"
#include "render/OpenGLShader.hpp"
#include <SDL3_image/SDL_image.h>
#include <glad/glad.h>
#include <unordered_map>

namespace cube::render {
void OpenGLRenderer::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void OpenGLRenderer::setClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}
IMesh *OpenGLRenderer::createMesh() { return create<OpenGLMesh>(); }

IShader *OpenGLRenderer::createShader(
    const std::string &name,
    const std::unordered_map<IShader::Type, std::string> &sources) {
  removeShader(name);
  auto shader = create<OpenGLShader>(sources);
  _shaders[name] = shader;
  return shader;
};

IShader *OpenGLRenderer::getShader(const std::string &name) {
  if (_shaders.contains(name)) {
    return _shaders.at(name);
  }
  return nullptr;
};

void OpenGLRenderer::removeShader(const std::string &name) {
  if (_shaders.contains(name)) {
    delete _shaders.at(name);
    _shaders.erase(name);
  }
};

void OpenGLRenderer::setViewport(int32_t x, int32_t y, uint32_t w, uint32_t h) {
  glViewport(x, y, w, h);
}

ITexture *OpenGLRenderer::createTexture(const std::string &name, uint32_t width,
                                        uint32_t height,
                                        const ITexture::FORMAT &format,
                                        void *data) {
  removeTexture(name);
  OpenGLTexture *texture = create<OpenGLTexture>();
  texture->setImage(width, height, format, data);
  _textures[name] = texture;
  return texture;
}
ITexture *OpenGLRenderer::getTexture(const std::string &name) {
  if (_textures.contains(name)) {
    return _textures.at(name);
  }
  return nullptr;
}
void OpenGLRenderer::removeTexture(const std::string &name) {
  if (_textures.contains(name)) {
    delete _textures[name];
    _textures.erase(name);
  }
}

void OpenGLRenderer::draw(IMesh *mesh) {
  auto material = mesh->getMaterial();
  auto geometory = mesh->getGeometory()->cast<OpenGLGeometory>();
  auto &shaderName = material->getShader();
  OpenGLShader *shader = nullptr;
  if (!_shaders.contains(shaderName)) {
    shader = _shaders.at("cube.shader.default");
  } else {
    shader = _shaders.at(shaderName);
  }
  if (!shader) {
    return;
  }
  if (_shader != shader) {
    glUseProgram(shader->getHandle());
  }
  auto &textures = material->getTextures();
  int idx = 0;
  for (auto &[slot, name] : textures) {
    OpenGLTexture *texture = nullptr;
    if (_textures.contains(name)) {
      texture = _textures.at(name);
    } else if (_textures.contains("cube.texture.missing")) {
      texture = _textures.at("cube.texture.missing");
    }
    if (texture) {
      glActiveTexture(GL_TEXTURE0 + idx);
      glBindTexture(GL_TEXTURE_2D, texture->getHandle());
      shader->set(slot, idx);
    }
    idx++;
  }
  glBindVertexArray(geometory->getHandle());
  glDrawElements(GL_TRIANGLES,
                 geometory->getIndexAttribute()->getSize() / sizeof(uint32_t),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
} // namespace cube::render