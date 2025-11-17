#pragma once
#include "core/Object.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class ModLoader : public core::Object {
private:
  struct Manifest {
    std::string name;
    std::string version;
    std::unordered_map<std::string, std::string> locales;
    std::vector<std::string> dependencies;
    std::vector<std::string> children;
    std::string script;
    bool isLoaded = false;
    bool isEnable = false;
    std::optional<std::string> error = std::nullopt;
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
  auto enableMod(const std::string &modName) -> bool;
  auto disableMod(const std::string &modName) -> bool;
};
} // namespace cube::runtime