#pragma once
#include "core/Event.hpp"
#include "runtime/IWindow.hpp"
namespace cube::runtime {
class EventWindowFocus : public core::Event<"cube.runtime.WindowFocus"> {
private:
  IWindow *_win;

public:
  EventWindowFocus(IWindow *win) : _win(win) {}
  IWindow *getWindow() { return _win; }
};
}; // namespace cube::runtime