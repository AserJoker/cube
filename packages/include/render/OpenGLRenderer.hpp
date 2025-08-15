#pragma once
#include "IMesh.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "Renderer.hpp"
#include "render/ITexture.hpp"
#include <unordered_map>
namespace cube::render {
class OpenGLRenderer : public Renderer {
private:
  std::unordered_map<std::string, OpenGLShader *> _shaders;
  std::unordered_map<std::string, OpenGLTexture *> _textures;
  OpenGLShader *_shader{};

public:
  IRenderer *setClearColor(float r, float g, float b, float a) override;
  IRenderer *clear() override;
  IRenderer *draw(ICamera *camera, IMesh *mesh) override;
  IRenderer *draw(const RenderMode &mode, IGeometory *geometory) override;
  IMesh *createMesh() override;
  IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) override;
  IShader *getShader(const std::string &name) override;
  IRenderer *removeShader(const std::string &name) override;
  IRenderer *setViewport(int32_t x, int32_t y, uint32_t w, uint32_t h) override;
  ITexture *createTexture(const std::string &name, uint32_t width,
                          uint32_t height, const ITexture::Format &format,
                          void *data) override;
  ITexture *getTexture(const std::string &name) override;
  IRenderer *removeTexture(const std::string &name) override;

  IRenderer *setShader(const std::string &shader) override;
  IShader *getShader() override;
  IRenderer *setMaterial(Material *material) override;
  IRenderer *enableDepthTest() override;
  IRenderer *disableDepthTest() override;
  IRenderer *enableAlphaTest() override;
  IRenderer *disableAlphaTest() override;
  IRenderer *enableStencilTest() override;
  IRenderer *disableStencilTest() override;
  IRenderer *enableBlend() override;
  IRenderer *disableBlend() override;
};
} // namespace cube::render