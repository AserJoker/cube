#pragma once
#include "IMesh.hpp"
#include "OpenGLGeometory.hpp"
#include "OpenGLMaterial.hpp"
namespace cube::render {
class OpenGLMesh : public IMesh {
private:
  OpenGLGeometory *_geometory;
  OpenGLMaterial *_material;

public:
  OpenGLMesh();
  IGeometory *getGeometory() override;
  IMaterial *getMaterial() override;
};
} // namespace cube::render