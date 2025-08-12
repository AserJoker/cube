#include "render/OpenGLMesh.hpp"
#include "render/OpenGLGeometory.hpp"
namespace cube::render {
OpenGLMesh::OpenGLMesh() {
  _geometory = create<OpenGLGeometory>();
  _material = create<Material>();
}
IGeometory *OpenGLMesh::getGeometory() { return _geometory; };
Material *OpenGLMesh::getMaterial() { return _material; };
} // namespace cube::render