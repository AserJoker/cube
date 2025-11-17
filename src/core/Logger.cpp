#include "core/Logger.hpp"
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
using namespace cube;
using namespace cube::core;

Logger::Logger(const std::string &filepath) {
  if (!filepath.empty()) {
    if (std::filesystem::exists(filepath)) {
      auto zone = std::chrono::current_zone();
      auto now = std::chrono::system_clock::now();
      auto zoned_time = std::chrono::zoned_time(zone, now);
      std::filesystem::rename(
          filepath,
          filepath + "." + std::format("{:%Y_%m_%d_%H_%M_%S}", zoned_time));
    }
    _out = std::ofstream(filepath);
    if (!_out->is_open()) {
      _out = std::nullopt;
      std::cerr << "Failed to open log file: " << filepath << std::endl;
    }
  }
}
Logger ::~Logger() {
  if (_out) {
    _out->close();
    _out = std::nullopt;
  }
}
auto Logger::setMask(const Level &level) -> void { _mask = level; }
auto Logger::getMask() const -> const Level & { return _mask; }
auto Logger::write(const Level &level, const std::string &message) -> void {
  std::ostream &out = _out ? _out.value() : std::cout;
  auto zone = std::chrono::current_zone();
  auto now = std::chrono::system_clock::now();
  auto zoned_time = std::chrono::zoned_time(zone, now);
  out << std::format("{:%Y-%m-%d %H:%M:%S %Z}", zoned_time) << " ";
  switch (level) {
  case Level::DEBUG:
    out << "[DEBUG]";
    break;
  case Level::INFO:
    out << "[INFO]";
    break;
  case Level::LOG:
    out << "[LOG]";
    break;
  case Level::WARN:
    out << "[WARN]";
    break;
  case Level::ERROR:
    out << "[ERROR]";
    break;
  case Level::PANIC:
    out << "[PANIC]";
    break;
  }
  out << " : " << message << std::endl;
}