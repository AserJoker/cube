#pragma once
#include "Type.hpp"
#include "Variable.hpp"
#include <functional>
#include <vector>
namespace cube::reflection {
class ObjectType : public Type {
public:
  using Method =
      std::function<Variable *(Variable *self, std::vector<Variable *> args)>;

private:
  std::string _name;

  std::unordered_map<std::string, std::function<Variable *(Variable *self)>>
      _properties;
  std::unordered_map<std::string, Method> _methods;
  std::function<Variable *(std::vector<Variable *> args)> _construct;

public:
  ObjectType(const std::string &name);

  ~ObjectType() override;

  const char *getTypeName() const override;

  ObjectType *
  construct(const std::function<Variable *(const std::vector<Variable *> &)>
                &constructor);
  ObjectType *method(const std::string &name, const Method &method);
  ObjectType *property(const std::string &name,
                       const std::function<Variable *(Variable *self)> &getter);
  Variable *createInstance(const std::vector<Variable *> &args = {});

  Variable *invoke(Variable *self, const std::string &name,
                   const std::vector<Variable *> &args = {});
                   
  Variable *getField(Variable *self, const std::string &name);

public:
  static ObjectType *get(const std::string &name);
};

} // namespace cube::reflection