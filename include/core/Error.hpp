#pragma once
#include <format>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stacktrace>
#include <stdexcept>
#include <utility>

namespace cube::core {
class Error : public std::runtime_error {
private:
  static auto printTrace(const std::stacktrace &trace) -> std::string {
    std::stringstream out;
    out << "\n";
    for (auto &frame : trace) {
      out << "  " << frame.description() << "[" << frame.source_file() << ":"
          << frame.source_line() << "]" << std::endl;
    }
    return out.str();
  }

public:
  template <class... Types>
  Error(std::format_string<Types...> fmt, Types &&...args)
      : std::runtime_error(std::format(fmt, std::forward<Types>(args)...) +
                           printTrace(std::stacktrace::current(1))) {}
};
} // namespace cube::core