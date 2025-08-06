#pragma once
#include "IMesh.hpp"
#include "IRenderer.hpp"
#include "OpenGLShader.hpp"
#include <unordered_map>
namespace cube::render {
class OpenGLRenderer : public IRenderer {
private:
  std::unordered_map<std::string, OpenGLShader *> _shaders;

public:
  void setClearColor(const glm::vec4 &color) override;
  void clear() override;
  void draw(IMesh *mesh) override;
  IMesh *createMesh() override;
  IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) override;
  IShader *getShader(const std::string &name) override;
  void removeShader(const std::string &name) override;
};
} // namespace cube::render