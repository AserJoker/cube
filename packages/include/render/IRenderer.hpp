#pragma once
#include "IMesh.hpp"
#include "IShader.hpp"
#include "core/Object.hpp"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
namespace cube::render {
class IRenderer : public core::Object {
public:
  virtual void setClearColor(const glm::vec4 &color) = 0;
  virtual void clear() = 0;
  virtual void draw(IMesh *mesh) = 0;
  virtual IMesh *createMesh() = 0;
  virtual IShader *createShader(
      const std::string &name,
      const std::unordered_map<IShader::Type, std::string> &sources) = 0;
  virtual IShader *getShader(const std::string &name) = 0;
  virtual void removeShader(const std::string &name) = 0;
};
} // namespace cube::render