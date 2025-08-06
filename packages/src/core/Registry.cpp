#include "core/Registry.hpp"
#include "core/Object.hpp"
#include <memory>
#include <unordered_map>
namespace cube::core {
static std::unordered_map<std::string, std::unique_ptr<Object>> store;

Object *Registry::get(const std::string &name) {
  if (store.contains(name)) {
    return store.at(name).get();
  }
  return nullptr;
}

void Registry::set(const std::string &name, std::unique_ptr<Object> &&object) {
  store[name].swap(object);
}

void Registry::remove(const std::string &name) { store.erase(name); }
} // namespace cube::core