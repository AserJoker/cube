#include "runtime/LoggerTarget.hpp"
#include <iostream>
using namespace cube;
using namespace cube::runtime;
auto LoggerTarget::overflow(int c) -> int {
  if (_streams.empty()) {
    return std::cout.rdbuf()->sputc(c);
  }
  int ret = 0;
  for (auto &stream : _streams) {
    ret |= stream->sputc(c);
  }
  return ret;
}
auto LoggerTarget::sync() -> int {
  if (_streams.empty()) {
    return std::cout.rdbuf()->pubsync();
  }
  int ret = 0;
  for (auto &stream : _streams) {
    ret |= stream->pubsync();
  }
  return ret;
}

auto LoggerTarget::addTarget(std::ostream &target) -> void {
  _streams.push_back(target.rdbuf());
}
auto LoggerTarget::addTarget(std::streambuf *target) -> void {
  _streams.push_back(target);
}
auto LoggerTarget::addTarget(const std::string &path) -> void {
  auto &file = _files[path];
  if (!file.is_open()) {
    file.open(path);
    if (!file.is_open()) {
      std::cerr << "Failed to open log file: " << path << std::endl;
    }
  }
  _streams.push_back(file.rdbuf());
}