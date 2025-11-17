#pragma once
#include <cstdint>
#include <optional>
#include <string>
namespace cube::core {
struct Version {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;
  std::string custom;
  bool operator>(const Version &another) const;
  bool operator==(const Version &another) const;
  static auto parse(const std::string &source) -> std::optional<Version>;
  static auto serialize(const Version &version) -> std::string;
};
} // namespace cube::core