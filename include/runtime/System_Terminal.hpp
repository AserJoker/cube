#pragma once
#include "core/Point.hpp"
#include "core/Size.hpp"
#include "runtime/System.hpp"
#include <map>
#include <queue>
#include <sys/types.h>
#include <termios.h>
#include <vector>
namespace cube::runtime {
class System_Terminal : public System {
public:
  struct Attribute {
    union {
      uint8_t value;
      struct {
        uint8_t bold : 1;
        uint8_t underline : 1;
        uint8_t blink : 1;
        uint8_t foreground_i256 : 1;
        uint8_t background_i256 : 1;
        uint8_t foreground_t24 : 1;
        uint8_t background_t24 : 1;
      };
    };
    uint32_t background;
    uint32_t foreground;
  };
  struct Pixel {
    uint32_t code;
    Attribute attr;
  };

private:
  std::queue<std::shared_ptr<Event>> _events;
  termios _term;
  core::Size _size;
  std::vector<Pixel> _pixels;
  std::map<int, std::map<int, Pixel>> _renderQueue;

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
  System_Terminal &draw(int x, int y, const char *str,
                        const Attribute &attr = {});
  System_Terminal &draw(int x, int y, uint32_t str, const Attribute &attr = {});
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