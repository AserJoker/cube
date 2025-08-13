#include "render/PerspectiveCamera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
namespace cube::render {
PerspectiveCamera::PerspectiveCamera(float fov, uint32_t width, uint32_t height,
                                     float near, float far) {
  float aspect = width * 1.0f / height;
  _zoom = height / 768.0f;
  getProjection() = glm::perspective(glm::radians(fov), aspect, near, far);
}
} // namespace cube::render