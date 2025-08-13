#pragma once
#include "IShader.hpp"
#include "render/Shader.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
namespace cube::render {
class OpenGLShader : public Shader {
private:
  uint32_t _handle;

public:
  OpenGLShader(const std::string &name,
               const std::unordered_map<IShader::Type, std::string> &sources);
  ~OpenGLShader() override;
  uint32_t getHandle() const;
  void set(const std::string &name, bool value) override;
  void set(const std::string &name, int value) override;
  void set(const std::string &name, float value) override;
  void set(const std::string &name, glm::vec2 value) override;
  void set(const std::string &name, glm::vec3 value) override;
  void set(const std::string &name, glm::vec4 value) override;
  void set(const std::string &name, glm::mat2 value) override;
  void set(const std::string &name, glm::mat3 value) override;
  void set(const std::string &name, glm::mat4 value) override;
};
} // namespace cube::render