#pragma once
namespace cube::runtime {
struct Event {
  size_t type;
  Event(size_t type) : type(type) {}
};
template <size_t T> struct SimpleEvent : public Event {
  SimpleEvent() : Event(T) {}
  static constexpr size_t type = T;
};
enum class EventType : size_t {
  QUIT,
};
using QuitEvent = SimpleEvent<(size_t)EventType::QUIT>;
}; // namespace cube::runtime