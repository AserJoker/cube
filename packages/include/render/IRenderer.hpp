#pragma once
#include "IGeometory.hpp"
#include "IMesh.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"
#include "Material.hpp"
#include "RenderMode.hpp"
#include "core/Object.hpp"
#include "render/ICamera.hpp"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace cube::render {
class IRenderer : public core::Object {
public:
  virtual IRenderer *setClearColor(float r, float g, float b, float a) = 0;
  virtual IRenderer *clear() = 0;
  virtual IRenderer *draw(ICamera *camera, IMesh *mesh) = 0;
  virtual IRenderer *draw(const RenderMode &mode, IGeometory *geometory) = 0;
  virtual IRenderer *setShader(const std::string &shader) = 0;
  virtual IShader *getShader() = 0;
  virtual IRenderer *setMaterial(Material *material) = 0;
  virtual IMesh *createMesh() = 0;
  virtual IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) = 0;
  virtual IShader *loadShader(const std::string &name,
                              const std::string &asset = "") = 0;
  virtual IShader *getShader(const std::string &name) = 0;
  virtual IRenderer *removeShader(const std::string &name) = 0;
  virtual IRenderer *setViewport(int32_t x, int32_t y, uint32_t w,
                                 uint32_t h) = 0;
  virtual ITexture *createTexture(const std::string &name, uint32_t width,
                                  uint32_t height,
                                  const ITexture::Format &format,
                                  void *data) = 0;
  virtual ITexture *loadTexture(const std::string &name,
                                const std::string &asset) = 0;
  virtual ITexture *getTexture(const std::string &name) = 0;
  virtual IRenderer *removeTexture(const std::string &name) = 0;
  virtual IRenderer *loadDefaultAssets() = 0;
  virtual IRenderer *enableDepthTest() = 0;
  virtual IRenderer *disableDepthTest() = 0;
  virtual IRenderer *enableAlphaTest() = 0;
  virtual IRenderer *disableAlphaTest() = 0;
  virtual IRenderer *enableStencilTest() = 0;
  virtual IRenderer *disableStencilTest() = 0;
  virtual IRenderer *enableBlend() = 0;
  virtual IRenderer *disableBlend() = 0;
};
} // namespace cube::render