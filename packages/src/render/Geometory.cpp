#include "render/Geometory.hpp"
#include "render/IGeometory.hpp"
namespace cube::render {

const glm::mat4 &Geometory::getMatrix() const { return _model; }
IGeometory *Geometory::setMatrix(const glm::mat4 &model) {
  _model = model;
  return this;
}
IGeometory *Geometory::applyMatrix(const glm::mat4 &mat) {
  _model = mat * _model;
  return this;
}
} // namespace cube::render