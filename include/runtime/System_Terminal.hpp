#pragma once
#include "core/Point.hpp"
#include "core/Size.hpp"
#include "runtime/System.hpp"
#include <queue>
#include <termios.h>
namespace cube::runtime {
class System_Terminal : public System {
private:
  std::queue<std::shared_ptr<Event>> _events;
  termios _term;
  core::Size _size;

private:
  void useAlternateScreen();
  void useMainScreen();
  void raw();
  core::Size getWindowSize() const;

private:
  static void onWindowSizeChange(int id);

public:
  System_Terminal();
  ~System_Terminal() override;
  std::shared_ptr<Event> recvEvent() override;
  void sendEvent(const std::shared_ptr<Event> &event) override;
  void clearScreen();
  const core::Size &getScreenSize() const;
  core::Point getCursorPosition() const;
  System_Terminal &move(int x, int y);
  System_Terminal &attr(uint8_t attr);
  System_Terminal &front(uint8_t r, uint8_t g, uint8_t b);
  System_Terminal &background(uint8_t r, uint8_t g, uint8_t b);
  System_Terminal &front(uint8_t idx);
  System_Terminal &background(uint8_t idx);
  System_Terminal &put(const char *s);
  System_Terminal &showCursor();
  System_Terminal &hideCursor();
  System_Terminal &enableCursorBlink();
  System_Terminal &disableCursorBlink();
};
} // namespace cube::runtime