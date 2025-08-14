#pragma once
#include "core/Event.hpp"
#include "runtime/IWindow.hpp"
namespace cube::runtime {
class EventWindowFocusLost
    : public core::Event<"cube.runtime.WindowFocusLost"> {
private:
  IWindow *_win;

public:
  EventWindowFocusLost(IWindow *win) : _win(win) {}
  IWindow *getWindow() { return _win; }
};
}; // namespace cube::runtime