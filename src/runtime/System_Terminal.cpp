
#include "runtime/System_Terminal.hpp"
#include "core/Size.hpp"
#include "runtime/Application.hpp"
#include "runtime/Event.hpp"
#include <cstdio>
#include <cstring>
#include <memory>
#include <thread>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <termios.h>
#endif

#define CSI "\x1b["
#define print(str) write(STDIN_FILENO, (str), sizeof(str))

using namespace cube;
using namespace cube::runtime;

void System_Terminal::onWindowSizeChange(int _) {
  auto &app = Application::getInstance();
  auto &system = dynamic_cast<System_Terminal &>(app.getSystem());
  system._size = system.getWindowSize();
}

System_Terminal::System_Terminal() {
  signal(SIGWINCH, &System_Terminal::onWindowSizeChange);
  tcgetattr(STDIN_FILENO, &_term);
  raw();
  clearScreen();
  _size = getWindowSize();
  hideCursor();
  move(10, 10).attr(1).attr(31).put("hello world");
}

System_Terminal::~System_Terminal() {
  showCursor();
  clearScreen();
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &_term);
  useMainScreen();
}
void System_Terminal::useAlternateScreen() { print(CSI "?1049h"); }

void System_Terminal::useMainScreen() { print(CSI "?1049l"); }

void System_Terminal::raw() {
  useAlternateScreen();
  termios term = _term;
  term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  term.c_oflag &= ~(OPOST);
  term.c_cflag |= (CS8);
  term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 0;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}
core::Size System_Terminal::getWindowSize() const {
  struct winsize ws = {};
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  return {ws.ws_col, ws.ws_row};
}

std::shared_ptr<Event> System_Terminal::recvEvent() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'q') {
      _events.push(std::make_shared<QuitEvent>());
    }
  }
  if (!_events.empty()) {
    auto event = _events.front();
    _events.pop();
    return event;
  }
  using namespace std::chrono;
  std::this_thread::sleep_for(4ms);
  return nullptr;
}

void System_Terminal::sendEvent(const std::shared_ptr<Event> &event) {
  _events.push(event);
}

void System_Terminal::clearScreen() {
  print(CSI "2J");
  print(CSI "H");
}
const core::Size &System_Terminal::getScreenSize() const { return _size; }

core::Point System_Terminal::getCursorPosition() const {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return {1, 1};
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[')
    return {1, 1};
  int x, y;
  if (sscanf(&buf[2], "%u;%u", &x, &y) != 2)
    return {1, 1};
  return {x, y};
}
System_Terminal &System_Terminal::move(int x, int y) {
  char s[16];
  size_t len = sprintf(s, CSI "%d;%dH", x, y);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::attr(uint8_t attr) {
  char s[16];
  size_t len = sprintf(s, CSI "%dm", attr);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::front(uint8_t r, uint8_t g, uint8_t b) {
  char s[16];
  size_t len = sprintf(s, CSI "38;2;%d;%d;%d", r, g, b);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::background(uint8_t r, uint8_t g, uint8_t b) {
  char s[16];
  size_t len = sprintf(s, CSI "48;2;%d;%d;%d", r, g, b);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::front(uint8_t idx) {
  char s[16];
  size_t len = sprintf(s, CSI "38;5;%d;", idx);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::background(uint8_t idx) {
  char s[16];
  size_t len = sprintf(s, CSI "48;5;%d;", idx);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::put(const char *s) {
  write(STDIN_FILENO, s, strlen(s));
  return *this;
}
System_Terminal &System_Terminal::showCursor() {
  print(CSI "?25h");
  return *this;
}
System_Terminal &System_Terminal::hideCursor() {
  print(CSI "?25l");
  return *this;
}
System_Terminal &System_Terminal::enableCursorBlink() {
  print(CSI "?12h");
  return *this;
}
System_Terminal &System_Terminal::disableCursorBlink() {
  print(CSI "?12l");
  return *this;
}