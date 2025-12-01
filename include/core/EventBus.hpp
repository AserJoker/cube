#pragma once
#include "core/Channel.hpp"
#include "core/Object.hpp"
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
namespace cube::core {
class EventBus : public Object {
public:
  struct Event : public Object {};
  using EventChannel = Channel<std::shared_ptr<Event>>;

private:
  std::unordered_map<std::string, std::unique_ptr<EventChannel>> _channels;
  std::mutex _channelMutex;

public:
  std::unique_ptr<EventChannel> &aquireChannel(const std::string &name) {
    {
      std::shared_lock<std::mutex> lock(_channelMutex);
      if (_channels.contains(name)) {
        return _channels.at(name);
      }
    }
    std::lock_guard<std::mutex> lock(_channelMutex);
    if (_channels.contains(name)) {
      return _channels.at(name);
    }
    _channels[name] = std::make_unique<EventChannel>();
    return _channels.at(name);
  }
  void emit(const std::string &channel, const std::shared_ptr<Event> &event) {
    std::shared_lock<std::mutex> lock(_channelMutex);
    if (_channels.contains(channel)) {
      _channels.at(channel)->send(event);
    }
  }
  void emit(const std::shared_ptr<Event> &event) {
    std::shared_lock<std::mutex> lock(_channelMutex);
    for (auto &[_, channel] : _channels) {
      channel->send(event);
    }
  }
};
} // namespace cube::core