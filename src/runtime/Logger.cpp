#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <streambuf>
#include <string>

using namespace cube;
using namespace cube::runtime;

Logger::Level Logger::_mask = Logger::Level::INFO;

Logger::Logger(const std::string &name, std::streambuf *rdbuf)
    : _name(name), _output(rdbuf) {}
auto Logger::setMask(const Level &mask) -> void {
  _mask = mask;
  switch (mask) {
  case Level::DEBUG:
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
    break;
  case Level::INFO:
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);
    break;
  case Level::LOG:
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);
    break;
  case Level::WARN:
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_WARN);
    break;
  case Level::ERROR:
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
    break;
  }
}
auto Logger::write(const Level &level, const std::string &message) -> void {
  if (level < _mask) {
    return;
  }
  std::stringstream ss;
  auto now = std::chrono::system_clock::now();
  auto zone = std::chrono::current_zone();
  auto zoned_time = std::chrono::zoned_time(zone, now);
  ss << std::format("{:%Y-%m-%d %H:%M:%OS} ", zoned_time);
  switch (level) {
  case Level::DEBUG:
    ss << "[DEBUG]";
    break;
  case Level::INFO:
    ss << "[INFO]";
    break;
  case Level::LOG:
    ss << "[LOG]";
    break;
  case Level::WARN:
    ss << "[WARN]";
    break;
  case Level::ERROR:
    ss << "[ERROR]";
    break;
  }
  ss << " [" << _name << "] :" << message;
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _output << ss.str() << std::endl;
  }
}