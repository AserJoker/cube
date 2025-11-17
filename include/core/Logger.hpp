#pragma once
#include "core/Object.hpp"
#include <format>
#include <fstream>
#include <optional>
#include <string>
#include <utility>
namespace cube::core {
class Logger : public Object {
public:
  enum class Level { DEBUG, INFO, LOG, WARN, ERROR, PANIC };

private:
  Level _mask = Level::INFO;
  void write(const Level &level, const std::string &source);
  std::optional<std::ofstream> _out = std::nullopt;

public:
  Logger(const std::string &filepath = "");
  ~Logger() override;
  auto setMask(const Level &level) -> void;
  auto getMask() const -> const Level &;
  template <class... Type>
  constexpr inline void debug(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::DEBUG) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }
  template <class... Type>
  constexpr inline void info(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::INFO) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }
  template <class... Type>
  constexpr inline void log(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::LOG) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }

  template <class... Type>
  constexpr inline void warn(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::WARN) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }

  template <class... Type>
  constexpr inline void error(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::ERROR) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }

  template <class... Type>
  constexpr inline void panic(std::format_string<Type...> fmt, Type &&...args) {
    if (_mask <= Level::ERROR) {
      write(Level::DEBUG, std::format(fmt, std::forward<Type>(args)...));
    }
  }
};
} // namespace cube::core