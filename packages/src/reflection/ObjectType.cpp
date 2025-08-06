#include "reflection/ObjectType.hpp"
#include <string>
#include <unordered_map>
namespace cube::reflection {
static std::unordered_map<std::string, ObjectType *> _clazzes;

ObjectType::ObjectType(const std::string &name)
    : Type(Type::Kind::OBJECT), _name(name) {
  _clazzes[name] = this;
}

ObjectType::~ObjectType() { _clazzes.erase(_name); }

const char *ObjectType::getTypeName() const { return _name.c_str(); }

ObjectType *ObjectType::construct(
    const std::function<Variable *(const std::vector<Variable *> &)>
        &constructor) {
  _construct = constructor;
  return this;
}
ObjectType *ObjectType::method(const std::string &name, const Method &method) {
  _methods[name] = method;
  return this;
}
ObjectType *
ObjectType::property(const std::string &name,
                     const std::function<Variable *(Variable *self)> &getter) {
  _properties[name] = getter;
  return this;
}
Variable *ObjectType::createInstance(const std::vector<Variable *> &args) {
  if (_construct) {
    return _construct(args);
  }
  return nullptr;
}

Variable *ObjectType::invoke(Variable *self, const std::string &name,
                             const std::vector<Variable *> &args) {
  if (_methods.contains(name)) {
    _methods[name](self, args);
  }
  return nullptr;
}
Variable *ObjectType::getField(Variable *self, const std::string &name) {
  if (_properties.contains(name)) {
    return _properties[name](self);
  }
  return nullptr;
}
ObjectType *ObjectType::get(const std::string &name) {
  if (_clazzes.contains(name)) {
    return _clazzes.at(name);
  }
  return nullptr;
}
} // namespace cube::reflection