#include "render/PerspectiveCamera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
namespace cube::render {
PerspectiveCamera::PerspectiveCamera(float fov, uint32_t width, uint32_t height,
                                     float near, float far)
    : _fov(fov), _aspect(width * 1.0f / height), _near(near), _far(far) {
  getProjection() = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
}
Camera *PerspectiveCamera::zoom(float zoom) {
  _fov += zoom;
  getProjection() = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
  return this;
}
} // namespace cube::render