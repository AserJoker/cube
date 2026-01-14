#ifndef _CUBE_CORE_VERSION_
#define _CUBE_CORE_VERSION_
#include <cstdint>
#include <format>
#include <sstream>
#include <string>
namespace cube::core {
struct Version {
  uint32_t major = {};
  uint32_t minor = {};
  uint32_t patch = {};
  std::string build = {};
  Version() = default;
  static Version parse(std::string source) {
    Version result = {};
    size_t idx = source.find_first_of('.');
    if (idx != std::string::npos) {
      std::string major = source.substr(0, idx);
      std::stringstream ss(major);
      ss >> result.major;
      source = source.substr(idx + 1);
    }
    idx = source.find_first_of('.');
    if (idx != std::string::npos) {
      std::string minor = source.substr(0, idx);
      std::stringstream ss(minor);
      ss >> result.minor;
      source = source.substr(idx + 1);
    }
    std::stringstream ss(source);
    ss >> result.patch;
    ss >> result.build;
    return result;
  }
  std::string toString() const {
    return std::format("{}.{}.{}{}", major, minor, patch, build);
  }
  bool operator<(const Version &another) const {
    if (major < another.major) {
      return true;
    }
    if (minor < another.minor) {
      return true;
    }
    if (patch < another.patch) {
      return true;
    }
    return false;
  }
  bool operator==(const Version &another) const {
    if (this == &another) {
      return true;
    }
    if (major == another.major && minor == another.minor &&
        patch == another.patch && build == another.build) {
      return true;
    }
    return false;
  }
};
} // namespace cube::core
#endif