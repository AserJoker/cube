#pragma once
#include "core/Object.hpp"
#include <string>
#include <unordered_map>
namespace cube::render {
class IMaterial : public core::Object {

public:
  virtual const std::string &getShader() const = 0;
  virtual void setShader(const std::string &name) = 0;
  virtual const std::unordered_map<std::string, std::string> &
  getTextures() const = 0;
  virtual void setTexture(const std::string &slot, const std::string &name) = 0;
  virtual void removeTexture(const std::string &slot) = 0;
  virtual bool hasTexture(const std::string &slot) const = 0;
  virtual const std::string &getTexture(const std::string &slot) const = 0;
};
}; // namespace cube::render