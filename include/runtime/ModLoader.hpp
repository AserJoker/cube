#pragma once
#include "core/Object.hpp"
#include "core/Version.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cube::runtime {
class ModLoader : public core::Object {
private:
  struct Manifest {
    std::string path;
    std::string name;
    core::Version version;
    core::Version gameVersion;
    std::unordered_map<std::string, std::string> locales;
    std::unordered_map<std::string, std::string> languages;
    std::unordered_map<std::string, std::string> dependencies;
    std::vector<std::string> children;
    std::string script;
    bool isEnable = false;
    bool isLoaded = false;
  };

  std::unordered_map<std::string, Manifest> _mods;

private:
  auto loadMod(const std::string &modName,
               std::vector<std::string> &dependences) -> void;

public:
  ModLoader();
  ~ModLoader() override;
  auto scanModList() -> void;
  auto loadAllMods() -> void;
  auto getManifest(const std::string &modName) -> std::optional<Manifest>;
  auto getModList() -> std::vector<std::string>;
  auto enableMod(const std::string &modName) -> void;
  auto disableMod(const std::string &modName) -> void;
};
} // namespace cube::runtime