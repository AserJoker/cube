#pragma once
#include "Type.hpp"
#include "Variable.hpp"
#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::reflection {
class ObjectType : public Type {
public:
  using Method =
      std::function<Variable *(Variable *self, std::vector<Variable *> args)>;

private:
  using Metadata = std::unordered_map<std::string, std::any>;

private:
  std::string _name;
  std::unordered_map<std::string, std::function<Variable *(Variable *self)>>
      _properties;
  std::unordered_map<std::string, Method> _methods;
  std::function<Variable *(std::vector<Variable *> args)> _construct;
  ObjectType *_extends;
  Metadata _classMetadatas;
  std::unordered_map<std::string, Metadata> _propertyMetadatas;

public:
  ObjectType(const std::string &name, ObjectType *extends = nullptr);
  ~ObjectType() override;
  const char *getTypeName() const override;
  ObjectType *getExtends();
  ObjectType *setExtends(ObjectType *extends);
  ObjectType *
  construct(const std::function<Variable *(const std::vector<Variable *> &)>
                &constructor);
  ObjectType *property(const std::string &name, const Method &method);
  ObjectType *property(const std::string &name,
                       const std::function<Variable *(Variable *self)> &getter);
  Variable *createInstance(const std::vector<Variable *> &args = {});
  Variable *invoke(Variable *self, const std::string &name,
                   const std::vector<Variable *> &args = {});
  Variable *getField(Variable *self, const std::string &name);
  std::vector<std::string> getFields() const;
  bool hasField(const std::string &name) const;
  std::vector<std::string> getMethods() const;
  bool hasMethod(const std::string &name) const;
  ObjectType *metadata(const std::string &name, const std::any &value);
  ObjectType *metadata(const std::string &property, const std::string &name,
                       const std::any &value);
  std::vector<std::string> getMetadatas();
  std::vector<std::string> getMetadatas(const std::string &property);
  template <class T> T *getMetadata(const std::string &name) {
    if (_classMetadatas.contains(name)) {
      if (typeid(T) == _classMetadatas[name].type()) {
        return std::any_cast<T *>(&_classMetadatas[name]);
      }
    }
    return nullptr;
  }

  template <class T>
  T *getMetadata(const std::string &property, const std::string &name) {
    if (_propertyMetadatas.contains(property)) {
      auto &metadatas = _propertyMetadatas.at(property);
      if (metadatas.contains(name)) {
        if (typeid(T) == metadatas[name].type()) {
          return std::any_cast<T *>(&metadatas[name]);
        }
      }
    }
    return nullptr;
  }

public:
  static ObjectType *get(const std::string &name);
  static std::vector<std::string> getClasses();
};

} // namespace cube::reflection