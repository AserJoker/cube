#pragma once
#include <algorithm>
#include <cstddef>
namespace cube::core {
template <std::size_t length> struct CompileString {
  char value[length];
  constexpr CompileString(const char (&init)[length]) {
    std::copy_n(init, length, value);
  }
};
} // namespace cube::core