#ifndef _H_CUBE_RUNTIME_LOGGER_
#define _H_CUBE_RUNTIME_LOGGER_
#include "core/Instance.hpp"
#include <SDL3/SDL_log.h>
#include <format>
#include <string>

namespace cube::runtime {
class Logger : public core::Instance {

public:
  enum class Level { DEBUG, INFO, WARN, ERR };

private:
  int _category;

private:
  auto write(const Level &level, const std::string &message) -> void;

public:
  Logger(const std::string &name);
  void setMask(const Level &mask);
  template <class... Args>
  auto debug(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::DEBUG, std::format(fmt, std::forward<Args>(args)...));
  }

  template <class... Args>
  auto info(std::format_string<Args...> fmt, Args &&...args) -> void {
    write(Level::INFO, std::format(fmt, std::forward<Args>(args)...));
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
  static void SDL_LogCallback(void *stream, int category,
                              SDL_LogPriority priority, const char *message);
};
} // namespace cube::runtime
#endif