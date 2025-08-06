#pragma once
#include "IAttribute.hpp"
#include "IGeometory.hpp"
#include "OpenGLBuffer.hpp"
#include <unordered_map>
namespace cube::render {
class OpenGLGeometory : public IGeometory {
private:
  uint32_t _handle;
  std::unordered_map<uint32_t, OpenGLBuffer *> _attributes;
  OpenGLBuffer *_indexAttribute;

public:
  OpenGLGeometory();
  ~OpenGLGeometory() override;
  IAttribute *createAttribute(uint32_t idx, bool normalize, size_t stride,
                              void *offset) override;
  IAttribute *getAttribute(uint32_t idx) override;
  void removeAttribute(uint32_t idx) override;
  void enableAttribute(uint32_t idx) override;
  void disableAttribute(uint32_t idx) override;
  IAttribute *getIndexAttribute() override;
  uint32_t getHandle() const;
};
}; // namespace cube::render