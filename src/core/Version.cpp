#include "core/Version.hpp"
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
using namespace cube;
using namespace cube::core;

bool Version::operator>(const Version &another) const {
  if (major > another.major) {
    return true;
  }
  if (major < another.major) {
    return false;
  }
  if (minor > another.minor) {
    return true;
  }
  if (minor < another.minor) {
    return false;
  }
  if (patch > another.patch) {
    return true;
  }
  if (patch < another.patch) {
    return false;
  }
  return false;
}
bool Version::operator==(const Version &another) const {
  return major == another.major && minor == another.minor &&
         patch == another.patch && custom == another.custom;
}
auto Version::parse(const std::string &source) -> std::optional<Version> {
  auto customPos = source.find('-');
  if (customPos == std::string::npos) {
    customPos = source.length();
  }
  auto majorPos = source.find('.');
  if (majorPos == std::string::npos) {
    return std::nullopt;
  }
  std::string major = source.substr(0, majorPos);
  auto minorPos = source.find('.', majorPos + 1);
  if (minorPos == std::string::npos) {
    return std::nullopt;
  }
  std::string minor = source.substr(majorPos + 1, minorPos - majorPos - 1);
  std::string patch = source.substr(minorPos + 1, customPos - minorPos - 1);
  Version version = {
      (uint32_t)std::atoi(major.c_str()),
      (uint32_t)std::atoi(minor.c_str()),
      (uint32_t)std::atoi(patch.c_str()),
  };
  if (customPos != source.length()) {
    version.custom = source.substr(customPos + 1);
  }
  return version;
}
auto Version::serialize(const Version &version) -> std::string {
  std::stringstream ss;
  ss << version.major << "." << version.minor << "." << version.patch;
  if (!version.custom.empty()) {
    ss << "-" << version.custom;
  }
  return ss.str();
}