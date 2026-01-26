#ifndef _H_CUBE_RUNTIME_LOGGER_
#define _H_CUBE_RUNTIME_LOGGER_
#include "core/Instance.hpp"
#include <format>
#include <iostream>
#include <mutex>
#include <ostream>
#include <streambuf>
#include <string>

namespace cube::runtime {
class Logger : public core::Instance {

public:
  enum class Level { DEBUG, INFO, LOG, WARN, ERR };

private:
  std::string _name;
  std::ostream _output;
  std::mutex _mutex;
  static Level _mask;

public:
  auto write(const Level &level, const std::string &message) -> void;

  template <class... Args>
  auto debug(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::DEBUG, std::format(fmt, std::forward<Args>(args)...));
  }

  template <class... Args>
  auto info(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::INFO, std::format(fmt, std::forward<Args>(args)...));
  }

  template <class... Args>
  auto log(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::LOG, std::format(fmt, std::forward<Args>(args)...));
  }

  template <class... Args>
  auto warn(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::WARN, std::format(fmt, std::forward<Args>(args)...));
  }

  template <class... Args>
  auto error(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::ERR, std::format(fmt, std::forward<Args>(args)...));
  }

public:
  Logger(const std::string &name, std::streambuf *rdbuf);

public:
  static auto setMask(const Level &mask) -> void;
};
} // namespace cube::runtime
#endif