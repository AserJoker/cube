#pragma once
#include "IRenderer.hpp"
namespace cube::render {
class Renderer : public IRenderer {
public:
  ITexture *loadTexture(const std::string &name,
                        const std::string &asset) override;
  IShader *loadShader(const std::string &name,
                      const std::string &asset = "") override;
  void loadDefaultAssets() override;
};
}; // namespace cube::render