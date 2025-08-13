#pragma once
#include "ICamera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
namespace cube::render {
class Camera : public ICamera {
private:
  glm::mat4 _projection = {1.0f};

  glm::vec3 _position = {0, 0, -3};
  glm::vec3 _direction = {0, 0, 1};
  glm::vec3 _up = {0, 1, 0};
  glm::mat4 _view = glm::lookAt(_position, _position + _direction, _up);

protected:
  glm::mat4 &getProjection();
  glm::mat4 &getView();

public:
  const glm::mat4 &getProjection() const;
  const glm::mat4 &getView() const;
  Camera *setPosition(const glm::vec3 &pos);
  Camera *setUp(const glm::vec3 &up);
  Camera *setDirection(const glm::vec3 &target);
};
}; // namespace cube::render