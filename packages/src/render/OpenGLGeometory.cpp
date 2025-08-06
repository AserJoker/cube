#include "render/OpenGLGeometory.hpp"
#include "render/OpenGLBuffer.hpp"
#include <glad/glad.h>
namespace cube::render {
OpenGLGeometory::OpenGLGeometory() : _handle(0), _indexAttribute(nullptr) {
  glGenVertexArrays(1, &_handle);
  _indexAttribute = create<OpenGLBuffer>();
  glBindVertexArray(_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexAttribute->getHandle());
  glBindVertexArray(0);
}
OpenGLGeometory::~OpenGLGeometory() {
  if (_handle) {
    glDeleteVertexArrays(1, &_handle);
  }
}

IAttribute *OpenGLGeometory::createAttribute(uint32_t idx, bool normalize,
                                             size_t stride, void *offset) {
  removeAttribute(idx);
  OpenGLBuffer *attribute = create<OpenGLBuffer>();
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

void OpenGLGeometory::removeAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    delete _attributes[idx];
    _attributes.erase(idx);
  }
};

void OpenGLGeometory::enableAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    glBindVertexArray(_handle);
    glEnableVertexAttribArray((uint32_t)idx);
    glBindVertexArray(0);
  }
};

void OpenGLGeometory::disableAttribute(uint32_t idx) {
  if (_attributes.contains(idx)) {
    glBindVertexArray(_handle);
    glDisableVertexAttribArray((uint32_t)idx);
    glBindVertexArray(0);
  }
};

IAttribute *OpenGLGeometory::getIndexAttribute() { return _indexAttribute; };

uint32_t OpenGLGeometory::getHandle() const { return _handle; };
}; // namespace cube::render