#include "render/Geometory.hpp"
#include "render/IGeometory.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
IGeometory *Geometory::rotate(float angle, const glm::vec3 &center) {
  _model = glm::rotate(_model, glm::radians(angle), center);
  return this;
};
IGeometory *Geometory::move(const glm::vec3 &offset) {
  _model = glm::translate(_model, offset);
  return this;
};
IGeometory *Geometory::scale(const glm::vec3 &scale) {
  _model = glm::scale(_model, scale);
  return this;
};
} // namespace cube::render