#pragma once
#include "ObjectType.hpp"
#include "Variable.hpp"
#include "core/Singleton.hpp"
#include <string>
#include <type_traits>
#include <vector>
namespace cube::reflection {
template <class T> class ClassType : public ObjectType {
private:
private:
  template <class V>
    requires std::is_integral_v<V>
  Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setInteger([=]() -> int64_t { return *v; },
                      [=](const int64_t &val) -> void { *v = val; });
    return value;
  }
  template <class V>
    requires std::is_floating_point_v<V>
  Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setNumber([=]() -> double { return *v; },
                     [=](const double &val) -> void { *v = val; });
    return value;
  }

  template <class V>
    requires std::is_same_v<V, bool>
  Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setBoolean(v);
    return value;
  }

  template <class V>
    requires std::is_same_v<V, std::string>
  Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setString(v);
    return value;
  }

  template <class V> Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setObject(core::Singleton<ClassType<V>>::get(), v);
    return value;
  }

  template <class Arg>
    requires std::is_integral_v<Arg>
  static Arg argument(std::vector<Variable *> &args) {
    Variable *arg = args.back();
    args.pop_back();
    return arg->getInteger();
  }

  template <class Arg>
    requires std::is_floating_point_v<Arg>
  static Arg argument(std::vector<Variable *> &args) {
    Variable *arg = args.back();
    args.pop_back();
    return arg->getNumber();
  }

  template <class Arg>
    requires std::is_same_v<Arg, bool>
  static Arg argument(std::vector<Variable *> &args) {
    Variable *arg = args.back();
    args.pop_back();
    return arg->getBoolean();
  }

  template <class Arg>
    requires std::is_same_v<Arg, std::string>
  static Arg argument(std::vector<Variable *> &args) {
    Variable *arg = args.back();
    args.pop_back();
    return arg->getString();
  }

  template <class Arg> static Arg argument(std::vector<Variable *> &args) {
    Variable *arg = args.back();
    args.pop_back();
    return arg->getObject()->cast<typename std::remove_pointer_t<Arg>>();
  }

private:
public:
  ClassType(const std::string &name = typeid(T).name()) : ObjectType(name) {}

  template <class... Args> ClassType *construct() {
    ObjectType::construct([this](std::vector<Variable *> args) -> Variable * {
      T *value = create<T>(argument<typename std::remove_cvref_t<Args>>()...);
      return resolve(value);
    });
    return this;
  }
  template <class R, class... Args>
  ClassType *method(const std::string &name, R (T::*callee)(Args... args)) {
    ObjectType::method(
        name,
        [=, this](Variable *self, std::vector<Variable *> args) -> Variable * {
          T *obj = self->getObject()->cast<T>();
          auto value = (obj->*callee)(
              argument<typename std::remove_cvref_t<Args>>(args)...);
          return resolve(value);
        });
    return this;
  }
  template <class... Args>
  ClassType *method(const std::string &name, void (T::*callee)(Args... args)) {
    ObjectType::method(
        name,
        [=, this](Variable *self, std::vector<Variable *> args) -> Variable * {
          T *obj = self->getObject()->cast<T>();
          (obj->*callee)(argument<typename std::remove_cvref_t<Args>>(args)...);
          return create<Variable>();
        });
    return this;
  }

  template <class F> ClassType *property(const std::string &name, F T::*field) {
    ObjectType::property(name, [=, this](Variable *self) -> Variable * {
      T *obj = self->getObject()->cast<T>();
      return resolve(&(obj->*field));
    });
    return this;
  }
};
}; // namespace cube::reflection