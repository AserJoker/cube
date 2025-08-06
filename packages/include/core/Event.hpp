#pragma once
#include "CompileString.hpp"
#include <string>
namespace cube::core {
class BaseEvent {
private:
  std::string _type;

public:
  virtual ~BaseEvent() = default;
  BaseEvent(const std::string &type) : _type(type) {}
  const std::string &getType() const { return _type; }
};
template <CompileString kind> class Event : public BaseEvent {
public:
  Event() : BaseEvent(kind.value) {}
};
} // namespace cube::core