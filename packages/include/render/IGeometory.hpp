#pragma once
#include "IAttribute.hpp"
#include "core/Object.hpp"
#include <cstdint>
namespace cube::render {
class IGeometory : public core::Object {
public:
  virtual IAttribute *createAttribute(uint32_t idx, bool normalize,
                                      size_t stride, void *offset) = 0;
  virtual IAttribute *getAttribute(uint32_t idx) = 0;
  virtual void removeAttribute(uint32_t idx) = 0;
  virtual void enableAttribute(uint32_t idx) = 0;
  virtual void disableAttribute(uint32_t idx) = 0;
  virtual IAttribute *getIndexAttribute() = 0;
};
}; // namespace cube::render