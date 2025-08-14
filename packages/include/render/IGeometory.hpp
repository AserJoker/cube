#pragma once
#include "IAttribute.hpp"
#include "core/Object.hpp"
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
namespace cube::render {
class IGeometory : public core::Object {
public:
  virtual IAttribute *createAttribute(uint32_t idx, bool normalize,
                                      size_t stride, void *offset) = 0;
  virtual IAttribute *getAttribute(uint32_t idx) = 0;
  virtual IGeometory *removeAttribute(uint32_t idx) = 0;
  virtual IGeometory *enableAttribute(uint32_t idx) = 0;
  virtual IGeometory *disableAttribute(uint32_t idx) = 0;
  virtual IAttribute *getIndexAttribute() = 0;
  virtual IAttribute *createIndexAttribute() = 0;
  virtual IGeometory *removeIndexAttribute() = 0;
  virtual const glm::mat4 &getMatrix() const = 0;
  virtual IGeometory *setMatrix(const glm::mat4 &model) = 0;
  virtual IGeometory *applyMatrix(const glm::mat4 &mat) = 0;
  virtual const std::unordered_map<uint32_t, IAttribute *> &
  getAttributes() const = 0;
  virtual IGeometory *rotate(float angle, const glm::vec3 &center) = 0;
  virtual IGeometory *move(const glm::vec3 &offset) = 0;
  virtual IGeometory *scale(const glm::vec3 &scale) = 0;
};
}; // namespace cube::render