#include "render/PerspectiveCamera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
namespace cube::render {
PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near,
                                     float far) {
  getProjection() = glm::perspective(glm::radians(fov), aspect, near, far);
}
} // namespace cube::render