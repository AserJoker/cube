#include "core/Logger.hpp"
#include <chrono>
#include <iostream>
#include <print>
namespace cube::core {
void Logger::write(const ILogger::Level &level, const std::string &message) {
  auto now = std::chrono::system_clock::now();
  auto local_time = std::chrono::zoned_time(std::chrono::current_zone(), now);
  std::string slevel;
  switch (level) {
  case ILogger::Level::DEBUG:
    slevel = "DEBUG";
    break;
  case ILogger::Level::INFO:
    slevel = "INFO";
    break;
  case ILogger::Level::LOG:
    slevel = "LOG";
    break;
  case ILogger::Level::WARN:
    slevel = "WARN";
    break;
  case ILogger::Level::ERROR:
    slevel = "ERROR";
    break;
  case ILogger::Level::PANIC:
    slevel = "PANIC";
    break;
  }
  std::println(std::cerr, "[ {} ][{:%Y-%m-%d %H:%M:%S}]: {}", slevel,
               local_time, message);
}
} // namespace cube::core