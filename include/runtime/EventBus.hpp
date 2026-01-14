#ifndef _H_CUBE_RUNTIME_EVENT_BUS_
#define _H_CUBE_RUNTIME_EVENT_BUS_
#include "core/Instance.hpp"
#include "core/Object.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cube::runtime {
class EventBus : public core::Instance {

private:
  template <typename Func> struct CallbackTraits;

  template <typename E> struct CallbackTraits<void(E *)> {
    using EventType = E;
  };

  template <typename E>
  struct CallbackTraits<void (*)(E *)> : CallbackTraits<void(E *)> {};

  template <typename E>
  struct CallbackTraits<std::function<void(E *)>> : CallbackTraits<void(E *)> {
  };

  template <typename ClassType, typename E>
  struct CallbackTraits<void (ClassType::*)(E *)> : CallbackTraits<void(E *)> {
  };

  template <typename ClassType, typename E>
  struct CallbackTraits<void (ClassType::*)(E *) const>
      : CallbackTraits<void(E *)> {};

  template <typename Callable> struct CallbackTraits {
  private:
    using CallType = decltype(&Callable::operator());

  public:
    using EventType = typename CallbackTraits<CallType>::EventType;
  };

public:
  class Subscribe : public core::Instance {
  private:
    uint32_t _id;
    std::string _type;

  public:
    Subscribe(const std::string &type, uint32_t id);
    void cancel();
    ~Subscribe() override;
  };

private:
  mutable std::shared_mutex _mutex;
  std::unordered_map<uint32_t, std::function<void(core::Object *)>> _callbacks;
  std::unordered_map<std::string, std::vector<uint32_t>> _subscribes;

  std::shared_ptr<Subscribe>
  subscribe(const std::string &type,
            const std::function<void(core::Object *)> &callback);

public:
  template <class Callback>
  std::shared_ptr<Subscribe> subscribe(Callback &&callback) {
    using E = CallbackTraits<Callback>::EventType;
    return subscribe(typeid(E).name(), [=](core::Object *event) {
      callback(dynamic_cast<E *>(event));
    });
  }

  template <class E, class C>
  std::shared_ptr<Subscribe> subscribe(C *self, void (C::*callback)(E *event)) {
    return subscribe(typeid(E).name(), [=](core::Object *event) {
      (self->*callback)(dynamic_cast<E *>(event));
    });
  }

  void unsubscribe(const std::string &type, uint32_t id);
  template <class E> void publish(auto &&...args) const {
    std::string type = typeid(E).name();
    std::shared_lock<std::shared_mutex> lock(_mutex);
    if (_subscribes.contains(type)) {
      auto &callbacks = _subscribes.at(type);
      if (callbacks.empty()) {
        return;
      }
      E event{std::forward<decltype(args)>(args)...};
      for (auto &id : callbacks) {
        if (_callbacks.contains(id)) {
          _callbacks.at(id)(&event);
        }
      }
    }
  }
};
} // namespace cube::runtime
#endif