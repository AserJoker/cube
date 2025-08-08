#pragma once
#include "Event.hpp"
#include "Object.hpp"
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cube::core {
class EventBus : public Object {
private:
  struct Callback {
    Object *self;
    std::function<void(BaseEvent &e)> handle;
    bool enable;
  };

private:
  std::unordered_map<std::string, std::vector<Callback>> _callbacks;

public:
  template <class E> void emit(auto &&...args) {
    auto type = typeid(E).name();
    if (_callbacks.contains(type)) {
      E event{std::forward<decltype(args)>(args)...};
      auto &callbacks = _callbacks.at(type);
      size_t idx = 0;
      while (idx != callbacks.size()) {
        if (!callbacks[idx].enable) {
          std::swap(callbacks[idx], callbacks.back());
          callbacks.pop_back();
        } else {
          callbacks[idx].handle(event);
          idx++;
        }
      }
    }
  }

  template <class T, class E> void on(T *self, void (T::*callback)(E &e)) {
    auto fn = [=](BaseEvent &e) -> void {
      (self->*callback)(dynamic_cast<E &>(e));
    };
    auto type = typeid(E).name();
    _callbacks[type].push_back({
        .handle = fn,
        .enable = true,
    });
  }

  template <class T, class E> void off(T *self, void (T::*callback)(E &e)) {
    auto type = typeid(E).name();
    if (_callbacks.contains(type)) {
      auto &callbacks = _callbacks.at(type);
      for (auto &cb : callbacks) {
        if (cb.self == self) {
          cb.enable = false;
          break;
        }
      }
    }
  }

  template <class T> void off(T *self) {
    for (auto &[_, callbacks] : _callbacks) {
      for (auto &cb : callbacks) {
        if (cb.self == self) {
          cb.enable = false;
        }
      }
    }
  }
};
} // namespace cube::core