#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>

using namespace cube;
using namespace cube::runtime;

static std::unordered_map<std::string, int> categorys = {
    {"SDL:APPLICATION", SDL_LOG_CATEGORY_APPLICATION},
    {"SDL:ERROR", SDL_LOG_CATEGORY_ERROR},
    {"SDL:ASSERT", SDL_LOG_CATEGORY_ASSERT},
    {"SDL:SYSTEM", SDL_LOG_CATEGORY_SYSTEM},
    {"SDL:AUDIO", SDL_LOG_CATEGORY_AUDIO},
    {"SDL:VIDEO", SDL_LOG_CATEGORY_VIDEO},
    {"SDL:RENDER", SDL_LOG_CATEGORY_RENDER},
    {"SDL:INPUT", SDL_LOG_CATEGORY_INPUT},
    {"SDL:TEST", SDL_LOG_CATEGORY_TEST},
    {"SDL:GPU", SDL_LOG_CATEGORY_GPU},
};

Logger::Logger(const std::string &name) {
  if (!categorys.contains(name)) {
    categorys[name] = categorys.size();
  }
  _category = categorys.at(name);
}

auto Logger::setMask(const Level &mask) -> void {
  switch (mask) {
  case Level::DEBUG:
    SDL_SetLogPriority(_category, SDL_LOG_PRIORITY_DEBUG);
    break;
  case Level::INFO:
    SDL_SetLogPriority(_category, SDL_LOG_PRIORITY_INFO);
    break;
  case Level::WARN:
    SDL_SetLogPriority(_category, SDL_LOG_PRIORITY_WARN);
    break;
  case Level::ERR:
    SDL_SetLogPriority(_category, SDL_LOG_PRIORITY_ERROR);
    break;
  }
}

auto Logger::write(const Level &level, const std::string &message) -> void {
  SDL_LogPriority priority = SDL_LOG_PRIORITY_INFO;
  switch (level) {
  case Level::DEBUG:
    priority = SDL_LOG_PRIORITY_DEBUG;
    break;
  case Level::INFO:
    priority = SDL_LOG_PRIORITY_INFO;
    break;
  case Level::WARN:
    priority = SDL_LOG_PRIORITY_WARN;
    break;
  case Level::ERR:
    priority = SDL_LOG_PRIORITY_ERROR;
    break;
  }
  SDL_LogMessage(_category, priority, "%s", message.c_str());
}
void Logger::SDL_LogCallback(void *stream, int category,
                             SDL_LogPriority priority, const char *message) {
  std::ostream &out = *(std::ostream *)stream;
  auto now = std::chrono::system_clock::now();
  auto zone = std::chrono::current_zone();
  auto zoned_time = std::chrono::zoned_time(zone, now);
  out << std::format("{:%Y-%m-%d %H:%M:%OS} ", zoned_time);
  switch (priority) {
  case SDL_LOG_PRIORITY_INVALID:
    out << "[INVALID] ";
    break;
  case SDL_LOG_PRIORITY_TRACE:
    out << "[TRACE] ";
    break;
  case SDL_LOG_PRIORITY_VERBOSE:
    out << "[VERBOSE] ";
    break;
  case SDL_LOG_PRIORITY_DEBUG:
    out << "[DEBUG] ";
    break;
  case SDL_LOG_PRIORITY_INFO:
    out << "[INFO] ";
    break;
  case SDL_LOG_PRIORITY_WARN:
    out << "[WARN] ";
    break;
  case SDL_LOG_PRIORITY_ERROR:
    out << "[ERROR] ";
    break;
  case SDL_LOG_PRIORITY_CRITICAL:
    out << "[CRITICAL] ";
    break;
  case SDL_LOG_PRIORITY_COUNT:
    out << "[COUNT] ";
    break;
  }
  for (auto &[name, cat] : categorys) {
    if (cat == category) {
      out << "[" + name << "] :";
    }
  }
  out << message;
  out << std::endl;
}