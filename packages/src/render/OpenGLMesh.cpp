#include "render/OpenGLMesh.hpp"
#include "render/OpenGLGeometory.hpp"
namespace cube::render {
OpenGLMesh::OpenGLMesh() {
  _geometory = create<OpenGLGeometory>();
  _material = create<OpenGLMaterial>();
}
IGeometory *OpenGLMesh::getGeometory() { return _geometory; };
IMaterial *OpenGLMesh::getMaterial() { return _material; };
} // namespace cube::render