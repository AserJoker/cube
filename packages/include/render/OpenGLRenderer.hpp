#pragma once
#include "IMesh.hpp"
#include "IRenderer.hpp"
#include "OpenGLShader.hpp"
#include <unordered_map>
namespace cube::render {
class OpenGLRenderer : public IRenderer {
private:
  std::unordered_map<std::string, OpenGLShader *> _shaders;
  OpenGLShader *_shader{};

public:
  void setClearColor(float r, float g, float b, float a) override;
  void clear() override;
  void draw(IMesh *mesh) override;
  IMesh *createMesh() override;
  IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) override;
  IShader *getShader(const std::string &name) override;
  void removeShader(const std::string &name) override;
  void setViewport(int32_t x, int32_t y, uint32_t w, uint32_t h) override;
};
} // namespace cube::render