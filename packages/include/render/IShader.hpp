#pragma once
#include "core/Object.hpp"
#include <glm/glm.hpp>
#include <string>
namespace cube::render {
class IShader : public core::Object {
public:
  enum class Type {
    COMPUTE,
    VERTEX,
    TESS_CONTROL,
    TESS_EVALUATION,
    GEOMETRY,
    FRAGMENT
  };

public:
  virtual const std::string &getName() const = 0;
  virtual void set(const std::string &name, bool value) = 0;
  virtual void set(const std::string &name, int value) = 0;
  virtual void set(const std::string &name, float value) = 0;
  virtual void set(const std::string &name, glm::vec2 value) = 0;
  virtual void set(const std::string &name, glm::vec3 value) = 0;
  virtual void set(const std::string &name, glm::vec4 value) = 0;
  virtual void set(const std::string &name, glm::mat2 value) = 0;
  virtual void set(const std::string &name, glm::mat3 value) = 0;
  virtual void set(const std::string &name, glm::mat4 value) = 0;
};
}; // namespace cube::render