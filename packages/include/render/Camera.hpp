#pragma once
#include "core/Object.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace cube::render {
class Camera : public core::Object {
private:
  glm::mat4 _projection = {1.0f};

  glm::vec3 _position = {0, 0, -1};
  glm::vec3 _direction = {0, 0, 1};
  glm::vec3 _up = {0, 1, 0};
  glm::mat4 _view = glm::lookAt(_position, _position + _direction, _up);

public:
  glm::mat4 &getProjection();
  glm::mat4 &getView();
  const glm::mat4 &getProjection() const;
  const glm::mat4 &getView() const;
  Camera *setPosition(const glm::vec3 &pos);
  Camera *setUp(const glm::vec3 &up);
  Camera *setDirection(const glm::vec3 &target);
};
}; // namespace cube::render