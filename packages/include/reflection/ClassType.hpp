#pragma once
#include "ObjectType.hpp"
#include "Type.hpp"
#include "Variable.hpp"
#include "core/Singleton.hpp"
#include <format>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>
namespace cube::reflection {
template <class T> class ClassType : public ObjectType {
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

  template <class V>
    requires std::is_convertible_v<V *, Variable *>
  Variable *resolve(V *v) {
    return v;
  }

  template <class V> Variable *resolve(V *v) {
    Variable *value = create<Variable>();
    value->setObject(core::Singleton<ClassType<V>>::get(), v);
    return value;
  }

  template <class Arg>
    requires std::is_integral_v<Arg>
  static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    if (arg->getType()->getKind() != Type::Kind::INTEGER) {
      throw std::runtime_error(
          std::format("Invalid parameter type, required integer, got {}",
                      arg->getType()->getTypeName()));
    }
    return arg->getInteger();
  }

  template <class Arg>
    requires std::is_floating_point_v<Arg>
  static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    if (arg->getType()->getKind() != Type::Kind::NUMBER) {
      throw std::runtime_error(
          std::format("Invalid parameter type, required number, got {}",
                      arg->getType()->getTypeName()));
    }
    return arg->getNumber();
  }

  template <class Arg>
    requires std::is_same_v<Arg, bool>
  static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    if (arg->getType()->getKind() != Type::Kind::STRING) {
      throw std::runtime_error(
          std::format("Invalid parameter type, required string, got {}",
                      arg->getType()->getTypeName()));
    }
    return arg->getBoolean();
  }

  template <class Arg>
    requires std::is_same_v<Arg, std::string>
  static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    if (arg->getType()->getKind() != Type::Kind::BOOLEAN) {
      throw std::runtime_error(
          std::format("Invalid parameter type, required boolean, got {}",
                      arg->getType()->getTypeName()));
    }
    return arg->getString();
  }

  template <class Arg>
    requires std::is_convertible_v<Arg *, Variable *>
  static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    return arg;
  }

  template <class Arg> static Arg argument(std::vector<Variable *> &args) {
    if (args.empty()) {
      return std::runtime_error("Invalid parameter");
    }
    Variable *arg = args.back();
    args.pop_back();
    if (arg->getType()->getKind() != Type::Kind::OBJECT) {
      throw std::runtime_error(
          std::format("Invalid parameter type, required object, got {}",
                      arg->getType()->getTypeName()));
    }
    return arg->getObject()->cast<typename std::remove_pointer_t<Arg>>();
  }

public:
  ClassType(const std::string &name = typeid(T).name()) : ObjectType(name) {}

  template <class... Args> ClassType *construct() {
    ObjectType::construct([this](std::vector<Variable *> args) -> Variable * {
      T *value = create<T>(argument<typename std::remove_cvref_t<Args>>()...);
      return resolve(value);
    });
    return this;
  }
  ClassType *singleton() {
    ObjectType::construct([this]() -> Variable * {
      T *value = core::Singleton<T>::get();
      return resolve(value);
    });
    return this;
  }
  ClassType *metadata(const std::string &name, const std::any &value) {
    ObjectType::metadata(name, value);
    return this;
  }
  template <class R, class... Args>
  ClassType *
  property(const std::string &name, R (T::*callee)(Args... args),
           const std::unordered_map<std::string, std::any> &metadatas = {}) {
    ObjectType::property(
        name,
        [=, this](Variable *self, std::vector<Variable *> args) -> Variable * {
          T *obj = self->getObject()->cast<T>();
          auto value = (obj->*callee)(
              argument<typename std::remove_cvref_t<Args>>(args)...);
          return resolve(value);
        });
    for (auto &[meta, value] : metadatas) {
      ObjectType::metadata(name, meta, value);
    }
    return this;
  }
  template <class... Args>
  ClassType *
  property(const std::string &name, void (T::*callee)(Args... args),
           const std::unordered_map<std::string, std::any> &metadatas = {}) {
    ObjectType::property(
        name,
        [=, this](Variable *self, std::vector<Variable *> args) -> Variable * {
          T *obj = self->getObject()->cast<T>();
          (obj->*callee)(argument<typename std::remove_cvref_t<Args>>(args)...);
          return create<Variable>();
        });
    for (auto &[meta, value] : metadatas) {
      ObjectType::metadata(name, meta, value);
    }
    return this;
  }

  template <class F>
  ClassType *
  property(const std::string &name, F T::*field,
           const std::unordered_map<std::string, std::any> &metadatas = {}) {
    ObjectType::property(name, [=, this](Variable *self) -> Variable * {
      T *obj = self->getObject()->cast<T>();
      return resolve(&(obj->*field));
    });
    for (auto &[meta, value] : metadatas) {
      ObjectType::metadata(name, meta, value);
    }
    return this;
  }
};
}; // namespace cube::reflection

#define CUBE_CLASS_DEF_BEGIN(clazz, ...)                                       \
  static inline void registerReflection() {                                    \
    using CURRENT_CLASS = clazz;                                               \
    core::Singleton<reflection::ClassType<CURRENT_CLASS>>::get()
#define CUBE_CLASS_EXTENDS(E)                                                  \
  ->setExtends(core::Singleton<reflection::ClassType<E>>::get())
#define CUBE_CONSTRUCTOR(args) ->construct<args>()
#define CUBE_PROPERTY(name, ...)                                               \
  ->property(#name, &CURRENT_CLASS::name, ##__VA_ARGS__)
#define CUBE_CLASS_DEF_END()                                                   \
  ;                                                                            \
  }

#define CUBE_CLASS_REFLECTION(clazz)                                           \
  struct class##Reflection {                                                   \
    class##Reflection() { clazz::registerReflection(); }                       \
  };                                                                           \
  static struct class##Reflection reflection;