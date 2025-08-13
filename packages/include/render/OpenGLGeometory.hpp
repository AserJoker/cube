#pragma once
#include "Geometory.hpp"
#include "IAttribute.hpp"
#include "IGeometory.hpp"
#include "OpenGLAttribute.hpp"
#include <cstdint>
#include <unordered_map>
namespace cube::render {
class OpenGLGeometory : public Geometory {
private:
  uint32_t _handle;
  std::unordered_map<uint32_t, IAttribute *> _attributes;
  OpenGLAttribute *_indexAttribute;

public:
  OpenGLGeometory();
  ~OpenGLGeometory() override;
  IAttribute *createAttribute(uint32_t idx, bool normalize, size_t stride,
                              void *offset) override;
  IAttribute *getAttribute(uint32_t idx) override;
  IGeometory *removeAttribute(uint32_t idx) override;
  IGeometory *enableAttribute(uint32_t idx) override;
  IGeometory *disableAttribute(uint32_t idx) override;
  IAttribute *getIndexAttribute() override;
  IAttribute *createIndexAttribute() override;
  IGeometory *removeIndexAttribute() override;
  uint32_t getHandle() const;
  const std::unordered_map<uint32_t, IAttribute *> &
  getAttributes() const override;
};
}; // namespace cube::render