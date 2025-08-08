#include "render/OpenGLRenderer.hpp"
#include "render/IMesh.hpp"
#include "render/OpenGLGeometory.hpp"
#include "render/OpenGLMesh.hpp"
#include "render/OpenGLShader.hpp"
#include <glad/glad.h>
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
void OpenGLRenderer::draw(IMesh *mesh) {
  auto material = mesh->getMerial();
  auto geometory = mesh->getGeometory()->cast<OpenGLGeometory>();
  auto &shaderName = material->getShader();
  OpenGLShader *shader = nullptr;
  if (!_shaders.contains(shaderName)) {
    shader = _shaders.at("cube.shader.default");
  } else {
    shader = _shaders.at(shaderName);
  }
  glUseProgram(shader->getHandle());
  glBindVertexArray(geometory->getHandle());
  glDrawElements(GL_TRIANGLES,
                 geometory->getIndexAttribute()->getSize() / sizeof(uint32_t),
                 GL_UNSIGNED_INT, 0);
  glUseProgram(0);
}
} // namespace cube::render