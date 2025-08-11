#pragma once
#include "CompileString.hpp"
#include <string>
namespace cube::core {
class BaseEvent {
private:
  std::string _type;
  bool _default;

public:
  virtual ~BaseEvent() = default;
  BaseEvent(const std::string &type) : _type(type), _default(true) {}
  const std::string &getType() const { return _type; }
  bool isDefault() const { return _default; }
  void preventDefault() { _default = false; }
};
template <CompileString kind> class Event : public BaseEvent {
public:
  Event() : BaseEvent(kind.value) {}
};
} // namespace cube::core