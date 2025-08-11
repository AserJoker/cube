#pragma once
#include "IMesh.hpp"
#include "IShader.hpp"
#include "ITexture.hpp"
#include "core/Object.hpp"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace cube::render {
class IRenderer : public core::Object {
public:
  virtual void setClearColor(float r, float g, float b, float a) = 0;
  virtual void clear() = 0;
  virtual void draw(IMesh *mesh) = 0;
  virtual IMesh *createMesh() = 0;
  virtual IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) = 0;
  virtual IShader *loadShader(const std::string &name,
                              const std::string &asset = "") = 0;
  virtual IShader *getShader(const std::string &name) = 0;
  virtual void removeShader(const std::string &name) = 0;
  virtual void setViewport(int32_t x, int32_t y, uint32_t w, uint32_t h) = 0;
  virtual ITexture *createTexture(const std::string &name, uint32_t width,
                                  uint32_t height,
                                  const ITexture::FORMAT &format,
                                  void *data) = 0;
  virtual ITexture *loadTexture(const std::string &name,
                                const std::string &asset) = 0;
  virtual ITexture *getTexture(const std::string &name) = 0;
  virtual void removeTexture(const std::string &name) = 0;
  virtual void loadDefaultAssets() = 0;
};
} // namespace cube::render