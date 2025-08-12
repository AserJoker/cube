#pragma once
#include "IMesh.hpp"
#include "OpenGLGeometory.hpp"
namespace cube::render {
class OpenGLMesh : public IMesh {
private:
  OpenGLGeometory *_geometory;
  Material *_material;

public:
  OpenGLMesh();
  IGeometory *getGeometory() override;
  Material *getMaterial() override;
};
} // namespace cube::render