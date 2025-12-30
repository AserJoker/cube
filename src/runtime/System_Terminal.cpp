
#include "runtime/System_Terminal.hpp"
#include "core/Size.hpp"
#include "core/wtswidth.h"
#include "runtime/Application.hpp"
#include "runtime/Event.hpp"
#include <cstdio>
#include <cstring>
#include <cwchar>
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
  system._pixels.resize(system._size.width * system._size.height);
}

System_Terminal::System_Terminal() {
  signal(SIGWINCH, &System_Terminal::onWindowSizeChange);
  tcgetattr(STDIN_FILENO, &_term);
  raw();
  clearScreen();
  _size = getWindowSize();
  hideCursor();
  _pixels.resize(_size.width * _size.height);
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

constexpr auto get_color_channel = [](const uint32_t color,
                                      size_t idx) -> uint8_t {
  return ((color) >> (idx * 8)) & 0xff;
};

size_t neo_utf32_to_utf8(uint32_t utf32, char *output) {
  char *s = output;
  s[0] = 0;
  if (utf32 < 0x7f) {
    s[0] = (uint8_t)utf32;
    return 1;
  } else if (utf32 < 0x7ff) {
    s[0] = (utf32 >> 6) | 0xC0;
    s[1] = (utf32 & 0x3F) | 0x80;
    return 2;
  } else if (utf32 < 0xFFFF) {
    s[0] = (utf32 >> 12) | 0xE0;
    s[1] = ((utf32 >> 6) & 0x3F) | 0x80;
    s[2] = (utf32 & 0x3F) | 0x80;
    return 3;
  } else if (utf32 < 0x10FFFF) {
    s[0] = (utf32 >> 18) | 0xF0;
    s[1] = ((utf32 >> 12) & 0x3F) | 0x80;
    s[2] = ((utf32 >> 6) & 0x3F) | 0x80;
    s[3] = (utf32 & 0x3F) | 0x80;
    return 4;
  }
  return 0;
}
int x = 0;
int y = 0;
std::shared_ptr<Event> System_Terminal::recvEvent() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'q') {
      _events.push(std::make_shared<QuitEvent>());
    }
    if (c == 'a') {
      x--;
    }
    if (c == 'd') {
      x++;
    }
    if (c == 'w') {
      y--;
    }
    if (c == 's') {
      y++;
    }
  }
  if (!_events.empty()) {
    auto event = _events.front();
    _events.pop();
    return event;
  }
  draw(x, y, "Hello world",
       {.underline = 1, .foreground_i256 = 1, .foreground = 3});
  using namespace std::chrono;
  std::this_thread::sleep_for(4ms);
  for (auto &[y, line] : _renderQueue) {
    for (auto &[x, pixel] : line) {
      move(x, y);
      if (pixel.attr.bold) {
        attr(1);
      }
      if (pixel.attr.underline) {
        attr(4);
      }
      if (pixel.attr.blink) {
        attr(6);
      }
      if (pixel.attr.background_t24) {
        background(get_color_channel(pixel.attr.background, 16),
                   get_color_channel(pixel.attr.background, 8),
                   get_color_channel(pixel.attr.background, 0));
      } else if (pixel.attr.background_i256) {
        background((uint8_t)pixel.attr.background);
      }
      if (pixel.attr.foreground_t24) {
        front(get_color_channel(pixel.attr.foreground, 16),
              get_color_channel(pixel.attr.foreground, 8),
              get_color_channel(pixel.attr.foreground, 0));
      } else if (pixel.attr.foreground) {
        front(pixel.attr.foreground);
      }
      char s[8];
      size_t len = neo_utf32_to_utf8(pixel.code, s);
      s[len] = 0;
      put(s);
    }
  }
  _renderQueue.clear();
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
  size_t len = sprintf(s, CSI "%d;%dH", y, x);
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
  size_t len = sprintf(s, CSI "38;2;%d;%d;%dm", r, g, b);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::background(uint8_t r, uint8_t g, uint8_t b) {
  char s[16];
  size_t len = sprintf(s, CSI "48;2;%d;%d;%dm", r, g, b);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::front(uint8_t idx) {
  char s[16];
  size_t len = sprintf(s, CSI "38;5;%dm", idx);
  write(STDIN_FILENO, s, len);
  return *this;
}
System_Terminal &System_Terminal::background(uint8_t idx) {
  char s[16];
  size_t len = sprintf(s, CSI "48;5;%dm", idx);
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

typedef struct _neo_utf8_char {
  const char *begin;
  const char *end;
} neo_utf8_char;

static neo_utf8_char neo_utf8_read_char(const char *str) {
  neo_utf8_char chr = {str, str};
  if (*str == 0) {
    return chr;
  }
  if ((*str & 0xe0) == 0xc0) {
    chr.end += 2;
  } else if ((*str & 0xf0) == 0xe0) {
    chr.end += 3;
  } else if ((*str & 0xf8) == 0xf0) {
    chr.end += 4;
  } else if ((*str & 0xfc) == 0xf8) {
    chr.end += 5;
  } else if ((*str & 0xfe) == 0xfc) {
    chr.end += 6;
  } else {
    chr.end += 1;
  }
  return chr;
}

static uint32_t neo_utf8_char_to_utf32(neo_utf8_char chr) {
  uint32_t value = 0;
  const char *s = chr.begin;
  if ((*s & 0b10000000) == 0b00000000) {
    value = *s;
  } else if ((*s & 0b11100000) == 0b11000000) {
    value = ((s[0] & 0b00011111) << 6) | (s[1] & 0b00111111);
  } else if ((*s & 0b11110000) == 0b11100000) {
    value = ((s[0] & 0b00001111) << 12) | ((s[1] & 0b00111111) << 6) |
            (s[2] & 0b00111111);
  } else if ((*s & 0b11111000) == 0b11110000) {
    value = ((s[0] & 0b00000111) << 18) | ((s[1] & 0b00111111) << 12) |
            ((s[2] & 0b00111111) << 6) | (s[3] & 0b00111111);
  }
  return value;
}

System_Terminal &System_Terminal::draw(int x, int y, const char *str,
                                       const Attribute &attr) {
  const char *ptr = str;
  int offset = 0;
  while (*ptr) {
    neo_utf8_char chr = neo_utf8_read_char(ptr);
    ptr = chr.end;
    auto idx = y * _size.width + x + offset;
    if (idx >= _pixels.size()) {
      break;
    }
    size_t width = wts8width(chr.begin, chr.end - chr.begin);
    offset += width;
    if (x + offset < 0 || y < 0) {
      continue;
    }
    uint32_t code = neo_utf8_char_to_utf32(chr);
    auto &cur = _pixels[idx];
    if (cur.code == code && cur.attr.value == attr.value &&
        cur.attr.foreground == attr.foreground &&
        cur.attr.background == attr.background) {
      continue;
    }
    cur.code = code;
    cur.attr = attr;
    _renderQueue[y][x + offset] = {code, attr};
    size_t i = 1;
    while (i < width) {
      _pixels[idx + i] = {};
      _renderQueue[y][x + offset + i] = {' ', {}};
      i++;
    }
  }
  return *this;
}