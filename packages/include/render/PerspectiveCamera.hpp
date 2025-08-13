#pragma once
#include "Camera.hpp"
namespace cube::render {
class PerspectiveCamera : public Camera {
private:
  float _fov;
  float _aspect;
  float _far;
  float _near;
  float _zoom;

public:
  PerspectiveCamera(float fov, uint32_t width, uint32_t height, float near,
                    float far);
};
} // namespace cube::render