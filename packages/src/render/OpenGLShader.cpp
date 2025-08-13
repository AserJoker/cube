#include "render/OpenGLShader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stdexcept>

namespace cube::render {

OpenGLShader::OpenGLShader(
    const std::string &name,
    const std::unordered_map<IShader::Type, std::string> &sources)
    : Shader(name) {
  _handle = glCreateProgram();
  for (auto &[type, src] : sources) {
    GLenum t;
    switch (type) {
    case IShader::Type::COMPUTE:
      t = GL_COMPUTE_SHADER;
      break;
    case IShader::Type::VERTEX:
      t = GL_VERTEX_SHADER;
      break;
    case IShader::Type::TESS_CONTROL:
      t = GL_TESS_CONTROL_SHADER;
      break;
    case IShader::Type::TESS_EVALUATION:
      t = GL_TESS_EVALUATION_SHADER;
      break;
    case IShader::Type::GEOMETRY:
      t = GL_GEOMETRY_SHADER;
      break;
    case IShader::Type::FRAGMENT:
      t = GL_FRAGMENT_SHADER;
      break;
    }
    uint32_t shader = glCreateShader(t);
    const char *source = src.c_str();
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      glDeleteShader(shader);
      throw std::runtime_error(std::string("Failed to compile shader:\n") +
                               infoLog);
    }
    glAttachShader(_handle, shader);
    glDeleteShader(shader);
  }
  glLinkProgram(_handle);
  int success;
  char infoLog[512];
  glGetProgramiv(_handle, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(_handle, 512, NULL, infoLog);
    throw std::runtime_error(std::string("Failed to link shader:\n") + infoLog);
  }
}
OpenGLShader::~OpenGLShader() {
  if (_handle) {
    glDeleteProgram(_handle);
  }
}
uint32_t OpenGLShader::getHandle() const { return _handle; }

void OpenGLShader::set(const std::string &name, bool value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform1i(loc, value);
}
void OpenGLShader::set(const std::string &name, int value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform1i(loc, value);
}
void OpenGLShader::set(const std::string &name, float value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform1f(loc, value);
}
void OpenGLShader::set(const std::string &name, glm::vec2 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform2fv(loc, 1, &value[0]);
}
void OpenGLShader::set(const std::string &name, glm::vec3 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform3fv(loc, 1, &value[0]);
}
void OpenGLShader::set(const std::string &name, glm::vec4 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniform4fv(loc, 1, &value[0]);
}
void OpenGLShader::set(const std::string &name, glm::mat2 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniformMatrix2fv(loc, 1, GL_FALSE, &value[0][0]);
}
void OpenGLShader::set(const std::string &name, glm::mat3 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
}
void OpenGLShader::set(const std::string &name, glm::mat4 value) {
  auto loc = glGetUniformLocation(_handle, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

} // namespace cube::render