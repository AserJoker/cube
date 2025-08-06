#pragma once
#include "Object.hpp"
#include <memory>
#include <string>
namespace cube::core {
class Registry : public Object {
public:
  static Object *get(const std::string &name);
  static void set(const std::string &name, std::unique_ptr<Object> &&object);
  static void remove(const std::string &name);
};
} // namespace cube::core