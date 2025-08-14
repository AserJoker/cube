#pragma once
#include "IGeometory.hpp"
namespace cube::render {
class Geometory : public IGeometory {
private:
  glm::mat4 _model{1.0f};

public:
  const glm::mat4 &getMatrix() const override;
  IGeometory *setMatrix(const glm::mat4& model) override;
  IGeometory *applyMatrix(const glm::mat4 &mat) override;
  IGeometory *rotate(float angle, const glm::vec3 &center) override;
  IGeometory *move(const glm::vec3 &offset) override;
  IGeometory *scale(const glm::vec3 &scale) override;
};
} // namespace cube::render