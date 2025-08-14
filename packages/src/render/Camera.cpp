#include "render/Camera.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
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
  _up = glm::normalize(up);
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::setDirection(const glm::vec3 &target) {
  _direction = glm::normalize(target);
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::move(const glm::vec3 &v) {
  glm::vec3 right = glm::normalize(glm::cross(_direction, _up));
  glm::vec3 speed = v.x * right + v.y * _up + v.z * _direction;
  _position += speed;
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}

Camera *Camera::yaw(float angle) {
  glm::vec4 dir{_direction, 1.0f};
  dir = glm::rotate(glm::mat4(1.0f), glm::radians(angle), _up) * dir;
  _direction = glm::normalize(glm::vec3(dir));
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::pitch(float angle) {
  glm::vec3 right = glm::normalize(glm::cross(_direction, _up));
  glm::vec4 dir(_direction, 1.0f);
  glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), right);
  dir = mat * dir;
  _direction = glm::normalize(glm::vec3(dir));
  _up = glm::normalize(glm::cross(right, _direction));
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
Camera *Camera::roll(float angle) {
  glm::vec4 up{_up, 1.0f};
  up = glm::rotate(glm::mat4(1.0f), glm::radians(angle), _direction) * up;
  _up = glm::normalize(glm::vec3(up));
  _view = glm::lookAt(_position, _position + _direction, _up);
  return this;
}
}; // namespace cube::render