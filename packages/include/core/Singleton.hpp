#pragma once
#include "Registry.hpp"
#include <memory>
namespace cube::core {
template <class T, bool autoInitialize = true> class Singleton final {
public:
  static T *get() {
    auto name = typeid(T).name();
    if constexpr (autoInitialize) {
      if (Registry::get(name) == nullptr) {
        auto ptr = std::make_unique<T>();
        Registry::set(name, std::move(ptr));
      }
    }
    return Registry::get(name)->cast<T>();
  }
  template <class Impl = T> static T *initialize(auto &&...args) {
    auto name = typeid(T).name();
    auto ptr = std::make_unique<Impl>(std::forward<decltype(args)>(args)...);
    Registry::set(name, std::move(ptr));
    return get();
  }
};
} // namespace cube::core