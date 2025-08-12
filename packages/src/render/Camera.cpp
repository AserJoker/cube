#include "render/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace cube::render {

glm::mat4 &Camera::getProjection() { return _projection; }
glm::mat4 &Camera::getView() { return _view; }
const glm::mat4 &Camera::getProjection() const { return _projection; }
const glm::mat4 &Camera::getView() const { return _view; }

Camera *Camera::setPosition(const glm::vec3 &pos) {
  _position = pos;
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::setUp(const glm::vec3 &up) {
  _up = up;
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::setDirection(const glm::vec3 &target) {
  _direction = target;
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
}; // namespace cube::render