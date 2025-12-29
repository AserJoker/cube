#pragma once
#include "core/Point.hpp"
#include "core/Size.hpp"
namespace cube::core {
struct Rect {
  union {
    Point point;
    struct {
      int x;
      int y;
    };
  };
  union {
    Size size;
    struct {
      uint32_t width;
      uint32_t height;
    };
  };
};
} // namespace cube::core