#ifndef _H_CUBE_RUNTIME_MOD_LOADER_
#define _H_CUBE_RUNTIME_MOD_LOADER_
#include "Mod.hpp"
#include "core/Instance.hpp"
#include <vector>

namespace cube::runtime {
class ModLoader : public core::Instance {
private:
  std::unordered_map<std::string, Mod> _mods;

private:
  void load(Mod &manifest, std::vector<std::string> &path);

public:
  auto reset() -> void;
  auto scanMods() -> void;
  auto getMod(const std::string &name) const -> const Mod *;
  auto getMod(const std::string &name) -> Mod *;
  auto getMods() const -> const std::unordered_map<std::string, Mod> &;
  auto loadEnableMods() -> void;
  auto enableMod(const std::string &name) -> void;
  auto disableMod(const std::string &name) -> void;
};
} // namespace cube::runtime
#endif