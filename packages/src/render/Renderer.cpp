#include "render/Renderer.hpp"
#include "core/Buffer.hpp"
#include "core/Singleton.hpp"
#include "runtime/AssetManager.hpp"
#include "serialization/json.hpp"
#include <SDL3_image/SDL_image.h>
namespace cube::render {
void Renderer::loadDefaultAssets() {
  loadShader("cube.shader.basic");
  auto missing =
      loadTexture("cube.texture.missing", "png:cube.texture.missing");
  if (missing) {
    missing->setNearestFilter();
  }
};
IShader *Renderer::loadShader(const std::string &name,
                              const std::string &asset) {
  static std::unordered_map<IShader::Type, std::string> names = {
      {IShader::Type::COMPUTE, "compute"},
      {IShader::Type::VERTEX, "vertex"},
      {IShader::Type::TESS_CONTROL, "tess_control"},
      {IShader::Type::TESS_EVALUATION, "tess_evaluation"},
      {IShader::Type::GEOMETRY, "geometry"},
      {IShader::Type::FRAGMENT, "fragment"},
  };
  core::Object defer;
  std::string path = asset;
  if (path.empty()) {
    path = name;
  }
  auto manger = core::Singleton<runtime::AssetManager>::get();
  auto manifestBuffer =
      manger->query("json:" + path + ".manifest")->cast<core::Buffer>();
  if (!manifestBuffer) {
    return nullptr;
  }
  auto manifest =
      serialization::Json::parse((const char *)manifestBuffer->getData());
  manifest->setParent(&defer);
  std::unordered_map<IShader::Type, std::string> sources;
  auto shaders = manifest->getField("shaders");
  if (shaders) {
    for (auto &[type, name] : names) {
      if (shaders->hasField(name)) {
        auto shaderFile = shaders->getField(name);
        std::string filename = shaderFile->getString();
        std::filesystem::path file = filename;
        std::string filetype = "unknown";
        if (file.has_extension()) {
          filetype = file.extension().string().substr(1);
          filename = filename.substr(0, filename.size() - filetype.size() - 1);
        }
        auto buf =
            manger->query(std::format("{}:{}.{}", filetype, path, filename))
                ->cast<core::Buffer>();
        if (buf) {
          sources[type] = (const char *)buf->getData();
        }
      }
    }
  }
  return createShader(name, sources);
}

ITexture *Renderer::loadTexture(const std::string &name,
                                const std::string &asset) {
  auto manger = core::Singleton<runtime::AssetManager>::get();
  auto buf = manger->query(asset)->cast<core::Buffer>();
  if (!buf) {
    return nullptr;
  }
  auto stream = SDL_IOFromConstMem(buf->getData(), buf->getLength());
  auto img = IMG_Load_IO(stream, true);
  ITexture *texture = nullptr;
  switch (img->format) {
  case SDL_PIXELFORMAT_INDEX8:
    texture = createTexture(name, img->w, img->h,
                            render::ITexture::FORMAT::GRAY, img->pixels);
    break;
  case SDL_PIXELFORMAT_RGB24:
    texture = createTexture(name, img->w, img->h,
                            render::ITexture::FORMAT::RGB24, img->pixels);
    break;
  default: {
    auto convert = SDL_ConvertSurface(img, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(img);
    img = convert;
  }
  case SDL_PIXELFORMAT_RGBA8888:
  case SDL_PIXELFORMAT_RGBA32:
    texture = createTexture(name, img->w, img->h,
                            render::ITexture::FORMAT::RGBA32, img->pixels);
    break;
  }
  SDL_DestroySurface(img);
  return texture;
}
}; // namespace cube::render