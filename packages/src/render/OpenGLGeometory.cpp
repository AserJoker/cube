#include "render/OpenGLGeometory.hpp"
#include "render/OpenGLAttribute.hpp"
#include <glad/glad.h>

namespace cube::render {
OpenGLGeometory::OpenGLGeometory() : _handle(0), _indexAttribute(nullptr) {
  glGenVertexArrays(1, &_handle);
}
OpenGLGeometory::~OpenGLGeometory() {
  if (_handle) {
    glDeleteVertexArrays(1, &_handle);
  }
}

IAttribute *OpenGLGeometory::createAttribute(uint32_t idx, bool normalize,
                                             size_t stride, void *offset) {
  removeAttribute(idx);
  OpenGLAttribute *attribute = create<OpenGLAttribute>(stride, normalize);
  glBindVertexArray(_handle);
  glBindBuffer(GL_ARRAY_BUFFER, attribute->getHandle());
  glVertexAttribPointer(idx, stride, GL_FLOAT, normalize,
                        stride * sizeof(float), offset);
  glBindVertexArray(0);
  _attributes[idx] = attribute;
  return attribute;
};

IAttribute *OpenGLGeometory::getAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    return _attributes.at(idx);
  }
  return nullptr;
};

IGeometory *OpenGLGeometory::removeAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    delete _attributes[idx];
    _attributes.erase(idx);
  }
  return this;
};

IGeometory *OpenGLGeometory::enableAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    glBindVertexArray(_handle);
    glEnableVertexAttribArray((uint32_t)idx);
    glBindVertexArray(0);
  }
  return this;
};

IGeometory *OpenGLGeometory::disableAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    glBindVertexArray(_handle);
    glDisableVertexAttribArray((uint32_t)idx);
    glBindVertexArray(0);
  }
  return this;
};

IAttribute *OpenGLGeometory::createIndexAttribute() {
  removeIndexAttribute();
  _indexAttribute = create<OpenGLAttribute>(1, false);
  glBindVertexArray(_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexAttribute->getHandle());
  glBindVertexArray(0);
  return _indexAttribute;
}

IGeometory *OpenGLGeometory::removeIndexAttribute() {
  if (_indexAttribute) {
    glBindVertexArray(_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    delete _indexAttribute;
  }
  return this;
};

IAttribute *OpenGLGeometory::getIndexAttribute() { return _indexAttribute; };

uint32_t OpenGLGeometory::getHandle() const { return _handle; };
const std::unordered_map<uint32_t, IAttribute *> &
OpenGLGeometory::getAttributes() const {
  return _attributes;
}
}; // namespace cube::render