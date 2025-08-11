#pragma once
#include "core/Object.hpp"
#include "render/IRenderer.hpp"
#include <string>
#include <tuple>
#include <vector>
namespace cube::runtime {
class IWindow : public core::Object {
public:
  virtual render::IRenderer *getRenderer() = 0;
  virtual void setTitle(const std::string &title) = 0;
  virtual const char *getTitle() const = 0;
  virtual void setPosition(int32_t x, int32_t y) = 0;
  virtual void setSize(uint32_t width, uint32_t height) = 0;
  virtual std::tuple<uint32_t, uint32_t> getSize() const = 0;
  virtual void present() = 0;
  virtual const std::string &
  information(const std::string &message,
              const std::vector<std::string> &buttons = {"Ok"},
              const std::string &title = "information") = 0;
  virtual const std::string &
  confirm(const std::string &message,
          const std::vector<std::string> &buttons = {"Ok", "Cancel"},
          const std::string &title = "confirm") = 0;
  virtual const std::string &
  error(const std::string &message,
        const std::vector<std::string> &buttons = {"Ok"},
        const std::string &title = "error") = 0;
  virtual const std::string &
  warn(const std::string &message,
       const std::vector<std::string> &buttons = {"Ok"},
       const std::string &title = "warn") = 0;
};
} // namespace cube::runtime