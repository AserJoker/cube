#pragma once
#include <format>
#include <iostream>
#include <ostream>
#include <stacktrace>
#include <stdexcept>
#include <utility>

namespace cube::core {
class Error : public std::runtime_error {
private:
  std::stacktrace _trace;

public:
  template <class... Types>
  Error(std::format_string<Types...> fmt, Types &&...args)
      : std::runtime_error(std::format(fmt, std::forward<Types>(args)...)),
        _trace(std::stacktrace::current(1)) {}
  void printTrace(std::ostream &out = std::cout) {
    for (auto &frame : _trace) {
      out << "  " << frame.description() << "[" << frame.source_file() << ":"
          << frame.source_line() << "]" << std::endl;
    }
  }
};
} // namespace cube::core