#include "render/Shader.hpp"
namespace cube::render {
Shader::Shader(const std::string &name) : _name(name) {}
const std::string &Shader::getName() const { return _name; }
} // namespace cube::render