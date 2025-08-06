#pragma once
#include "Object.hpp"
#include <format>
#include <string>
#include <utility>
namespace cube::core {
class ILogger : public Object {
public:
  enum class Level : uint8_t { DEBUG, INFO, LOG, WARN, ERROR, PANIC };

private:
  Level _mask = {Level::LOG};

protected:
  virtual void write(const Level &level, const std::string &message) = 0;

public:
  void setMask(const Level &level) { _mask = level; }

  const Level &getMask() const { return _mask; }

  template <typename... Args>
  void debug(std::format_string<Args...> fmt, Args &&...args) {

    write(Level::DEBUG, std::format(fmt, std::forward<Args>(args)...));
  };
  template <typename... Args>
  void info(std::format_string<Args...> fmt, Args &&...args) {
    if (_mask < Level::INFO) {
      return;
    }
    write(Level::INFO, std::format(fmt, std::forward<Args>(args)...));
  };
  template <typename... Args>
  void log(std::format_string<Args...> fmt, Args &&...args) {
    if (_mask < Level::LOG) {
      return;
    }
    write(Level::LOG, std::format(fmt, std::forward<Args>(args)...));
  };
  template <typename... Args>
  void warn(std::format_string<Args...> fmt, Args &&...args) {
    if (_mask < Level::WARN) {
      return;
    }
    write(Level::WARN, std::format(fmt, std::forward<Args>(args)...));
  };
  template <typename... Args>
  void error(std::format_string<Args...> fmt, Args &&...args) {
    if (_mask < Level::ERROR) {
      return;
    }
    write(Level::ERROR, std::format(fmt, std::forward<Args>(args)...));
  };
  template <typename... Args>
  void panic(std::format_string<Args...> fmt, Args &&...args) {
    if (_mask < Level::PANIC) {
      return;
    }
    write(Level::PANIC, std::format(fmt, std::forward<Args>(args)...));
  };
};

} // namespace cube::core