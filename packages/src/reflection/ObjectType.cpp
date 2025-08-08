#include "reflection/ObjectType.hpp"
#include <format>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::reflection {
static std::unordered_map<std::string, ObjectType *> _classes;

ObjectType::ObjectType(const std::string &name, ObjectType *extends)
    : Type(Type::Kind::OBJECT), _name(name), _extends(extends) {
  _classes[name] = this;
}

ObjectType::~ObjectType() { _classes.erase(_name); }

ObjectType *ObjectType::getExtends() { return _extends; }

ObjectType *ObjectType::setExtends(ObjectType *extends) {
  _extends = extends;
  return this;
}

const char *ObjectType::getTypeName() const { return _name.c_str(); }

ObjectType *ObjectType::construct(
    const std::function<Variable *(const std::vector<Variable *> &)>
        &constructor) {
  _construct = constructor;
  return this;
}

ObjectType *ObjectType::property(const std::string &name,
                                 const Method &method) {
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
  throw std::runtime_error(std::format("Type '{}' has not constructor", _name));
}

Variable *ObjectType::invoke(Variable *self, const std::string &name,
                             const std::vector<Variable *> &args) {
  if (_methods.contains(name)) {
    return _methods[name](self, args);
  }
  if (_extends) {
    return _extends->invoke(self, name, args);
  }
  throw std::runtime_error(
      std::format("Method '{}' is not found in '{}'", name, _name));
}
Variable *ObjectType::getField(Variable *self, const std::string &name) {
  if (_properties.contains(name)) {
    return _properties[name](self);
  }
  if (_extends) {
    return _extends->getField(self, name);
  }
  throw std::runtime_error(
      std::format("Field '{}' is not found in '{}'", name, _name));
}

std::vector<std::string> ObjectType::getFields() const {
  std::vector<std::string> fields;
  if (_extends) {
    fields = _extends->getFields();
  }
  for (auto &[name, _] : _properties) {
    std::erase(fields, name);
    fields.push_back(name);
  }
  return fields;
}
bool ObjectType::hasField(const std::string &name) const {
  if (_properties.contains(name)) {
    return true;
  }
  if (_extends) {
    return _extends->hasField(name);
  }
  return false;
}
std::vector<std::string> ObjectType::getMethods() const {
  std::vector<std::string> methods;
  if (_extends) {
    methods = _extends->getMethods();
  }
  for (auto &[name, _] : _methods) {
    std::erase(methods, name);
    methods.push_back(name);
  }
  return methods;
}
bool ObjectType::hasMethod(const std::string &name) const {
  if (_methods.contains(name)) {
    return true;
  }
  if (_extends) {
    return _extends->hasMethod(name);
  }
  return false;
}
ObjectType *ObjectType::metadata(const std::string &name,
                                 const std::any &value) {
  _classMetadatas[name] = value;
  return this;
}
ObjectType *ObjectType::metadata(const std::string &property,
                                 const std::string &name,
                                 const std::any &value) {
  _propertyMetadatas[property][name] = value;
  return this;
}
std::vector<std::string> ObjectType::getMetadatas() {
  std::vector<std::string> metadatas;
  for (auto &[name, _] : _classMetadatas) {
    metadatas.push_back(name);
  }
  return metadatas;
}
std::vector<std::string> ObjectType::getMetadatas(const std::string &property) {
  std::vector<std::string> metadatas;
  if (_propertyMetadatas.contains(property)) {
    for (auto &[name, _] : _propertyMetadatas[property]) {
      metadatas.push_back(name);
    }
  }
  return metadatas;
}
ObjectType *ObjectType::get(const std::string &name) {
  if (_classes.contains(name)) {
    return _classes.at(name);
  }
  return nullptr;
}
std::vector<std::string> ObjectType::getClasses() {
  std::vector<std::string> classes;
  for (auto &[name, _] : _classes) {
    classes.push_back(name);
  }
  return classes;
}
} // namespace cube::reflection