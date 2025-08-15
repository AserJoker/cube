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
IRenderer *OpenGLRenderer::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return this;
}
IRenderer *OpenGLRenderer::setClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  return this;
}
IMesh *OpenGLRenderer::createMesh() { return create<OpenGLMesh>(); }

IShader *OpenGLRenderer::createShader(
    const std::string &name,
    const std::unordered_map<IShader::Type, std::string> &sources) {
  removeShader(name);
  auto shader = create<OpenGLShader>(name, sources);
  _shaders[name] = shader;
  return shader;
};

IShader *OpenGLRenderer::getShader(const std::string &name) {
  if (_shaders.contains(name)) {
    return _shaders.at(name);
  }
  return nullptr;
};

IRenderer *OpenGLRenderer::removeShader(const std::string &name) {
  if (_shaders.contains(name)) {
    delete _shaders.at(name);
    _shaders.erase(name);
  }
  return this;
};

IRenderer *OpenGLRenderer::setViewport(int32_t x, int32_t y, uint32_t w,
                                       uint32_t h) {
  glViewport(x, y, w, h);
  return this;
}

ITexture *OpenGLRenderer::createTexture(const std::string &name, uint32_t width,
                                        uint32_t height,
                                        const ITexture::Format &format,
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
IRenderer *OpenGLRenderer::removeTexture(const std::string &name) {
  if (_textures.contains(name)) {
    delete _textures[name];
    _textures.erase(name);
  }
  return this;
}

IRenderer *OpenGLRenderer::setShader(const std::string &name) {
  if (_shader && _shader->getName() == name) {
    return this;
  }
  OpenGLShader *shader = nullptr;
  if (_shaders.contains(name)) {
    shader = _shaders.at(name);
  } else if (_shaders.contains("cube.shader.default")) {
    shader = _shaders.at("cube.shader.default");
  }
  _shader = shader;
  if (_shader) {
    glUseProgram(_shader->getHandle());
  }
  return this;
}
IShader *OpenGLRenderer::getShader() { return _shader; }

IRenderer *OpenGLRenderer::setMaterial(Material *material) {
  setShader(material->getShader());
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
      _shader->set(slot, idx);
    }
    idx++;
  }
  if (material->isAlphaTest()) {
    enableAlphaTest();
  } else {
    disableAlphaTest();
  }
  if (material->isDepthTest()) {
    enableDepthTest();
  } else {
    disableDepthTest();
  }
  if (material->isStencilTest()) {
    enableStencilTest();
  } else {
    disableStencilTest();
  }
  if (material->isBlend()) {
    enableBlend();
  } else {
    disableBlend();
  }
  return this;
}
IRenderer *OpenGLRenderer::enableDepthTest() {
  glEnable(GL_DEPTH_TEST);
  return this;
}
IRenderer *OpenGLRenderer::disableDepthTest() {
  glDisable(GL_DEPTH_TEST);
  return this;
}
IRenderer *OpenGLRenderer::enableAlphaTest() {
  glEnable(GL_ALPHA_TEST);
  return this;
}
IRenderer *OpenGLRenderer::disableAlphaTest() {
  glDisable(GL_ALPHA_TEST);
  return this;
}
IRenderer *OpenGLRenderer::enableStencilTest() {
  glEnable(GL_STENCIL_TEST);
  return this;
}
IRenderer *OpenGLRenderer::disableStencilTest() {
  glDisable(GL_STENCIL_TEST);
  return this;
}
IRenderer *OpenGLRenderer::enableBlend() {
  glEnable(GL_BLEND);
  return this;
};
IRenderer *OpenGLRenderer::disableBlend() {
  glDisable(GL_BLEND);
  return this;
};

IRenderer *OpenGLRenderer::draw(ICamera *camera, IMesh *mesh) {
  auto material = mesh->getMaterial();
  auto geometory = mesh->getGeometory()->cast<OpenGLGeometory>();
  setMaterial(material);
  if (!_shader) {
    return this;
  }
  if (camera) {
    _shader->set("projection", camera->getProjection());
    _shader->set("view", camera->getView());
  } else {
    _shader->set("projection", glm::mat4(1.0f));
    _shader->set("view", glm::mat4(1.0f));
  }
  draw(material->getRenderMode(), geometory);
  return this;
}
IRenderer *OpenGLRenderer::draw(const RenderMode &mode, IGeometory *geometory) {
  auto geo = geometory->cast<OpenGLGeometory>();
  _shader->set("model", geo->getMatrix());
  GLenum m;
  switch (mode) {
  case RenderMode::POINTS:
    m = GL_POINTS;
    break;
  case RenderMode::LINE_STRIP:
    m = GL_LINE_STRIP;
    break;
  case RenderMode::LINE_LOOP:
    m = GL_LINE_LOOP;
    break;
  case RenderMode::LINES:
    m = GL_LINES;
    break;
  case RenderMode::LINE_STRIP_ADJACENCY:
    m = GL_LINE_STRIP_ADJACENCY;
    break;
  case RenderMode::LINES_ADJACENCY:
    m = GL_LINES_ADJACENCY;
    break;
  case RenderMode::TRIANGLE_STRIP:
    m = GL_TRIANGLE_STRIP;
    break;
  case RenderMode::TRIANGLE_FAN:
    m = GL_TRIANGLE_FAN;
    break;
  case RenderMode::TRIANGLES:
    m = GL_TRIANGLES;
    break;
  case RenderMode::TRIANGLE_STRIP_ADJACENCY:
    m = GL_TRIANGLE_STRIP_ADJACENCY;
    break;
  case RenderMode::TRIANGLES_ADJACENCY:
    m = GL_TRIANGLES_ADJACENCY;
    break;
  case RenderMode::PATCHES:
    m = GL_PATCHES;
    break;
  }
  glBindVertexArray(geo->getHandle());
  if (geometory->getIndexAttribute()) {
    glDrawElements(m,
                   geometory->getIndexAttribute()->getSize() / sizeof(uint32_t),
                   GL_UNSIGNED_INT, 0);
  } else {
    auto &[_, attr] = *geometory->getAttributes().begin();
    glDrawArrays(m, 0, attr->getSize() / (sizeof(float) * attr->getStride()));
  }
  glBindVertexArray(0);
  return this;
}
} // namespace cube::render