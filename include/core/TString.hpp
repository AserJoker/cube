#ifndef _H_CUBE_CORE_TSTRING_
#define _H_CUBE_CORE_TSTRING_
#include <algorithm>
#include <cstddef>
namespace cube::core {
template <size_t N> struct TString {
  char value[N] = {};
  constexpr TString(const char (&src)[N]) { std::copy_n(src, N, value); }
  const char *toString() const { return value; }
};

} // namespace cube::core
#endif