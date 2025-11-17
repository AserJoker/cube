#include "runtime/ModLoader.hpp"
#include "core/Error.hpp"
#include "core/Json.hpp"
#include "runtime/Application.hpp"
#include <filesystem>
#include <optional>
using namespace cube;
using namespace cube::runtime;
ModLoader::ModLoader() {}
ModLoader::~ModLoader() {}
auto ModLoader::scanModList() -> void {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto moddir = app.getApplicationName() + ":mods/";
  auto path = asset.resolvePath(moddir);
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  if (!std::filesystem::is_directory(path)) {
    throw core::Error("Mod directory {} is not a directory", moddir);
  }
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_directory()) {
      auto modName = entry.path().filename().string();
      auto manifestPath = moddir + modName + "/manifest.json";
      auto manifestObj = asset.loadAs<core::Json>(manifestPath)->value;
      if (manifestObj.getType() != core::Value::Type::Object) {
        throw core::Error("Invalid mod manifest {} : invalid format, manifest "
                          "must be object",
                          manifestPath);
      }
      Manifest manifest;
      auto manifestMap = manifestObj.asObject();
      if (manifestMap->contains("name")) {
        auto nameVal = manifestMap->at("name").asString();
        if (nameVal) {
          manifest.name = nameVal.value();
        } else {
          throw core::Error("Invalid mod manifest {} : invalid format, field "
                            "'name' is required",
                            manifestPath);
        }
      }
      if (manifestMap->contains("version")) {
        auto versionVal = manifestMap->at("version").asString();
        if (versionVal) {
          manifest.version = versionVal.value();
        } else {
          throw core::Error("Invalid mod manifest {} : invalid format, field "
                            "'version' is required",
                            manifestPath);
        }
      }
      if (manifestMap->contains("locales")) {
        auto localesVal = manifestMap->at("locales").asObject();
        if (localesVal) {
          for (const auto &[key, val] : localesVal.value()) {
            auto localeStr = val.asString();
            if (localeStr) {
              manifest.locales[key] = localeStr.value();
            } else {
              throw core::Error("Invalid mod manifest {} : invalid format, "
                                "field 'locales' must be string map",
                                manifestPath);
            }
          }
        }
      }
      if (manifestMap->contains("dependencies")) {
        auto dependenciesVal = manifestMap->at("dependencies").asArray();
        if (dependenciesVal) {
          for (const auto &depVal : dependenciesVal.value()) {
            auto depStr = depVal.asString();
            if (depStr) {
              manifest.dependencies.push_back(depStr.value());
            }
          }
        }
      }
      if (manifestMap->contains("script")) {
        auto scriptVal = manifestMap->at("script").asString();
        if (scriptVal) {
          manifest.script = scriptVal.value();
        }
      }
      _mods[manifest.name] = manifest;
      asset.addDomain(manifest.name, path + modName + "/");
    }
  }
}
auto ModLoader::loadAllMods() -> void {
  std::vector<std::string> dependences;
  for (const auto &[modName, manifest] : _mods) {
    loadMod(modName, dependences);
  }
}
auto ModLoader::loadMod(const std::string &modName,
                        std::vector<std::string> &dependences) -> void {}

auto ModLoader::getManifest(const std::string &modName)
    -> std::optional<Manifest> {
  if (_mods.contains(modName)) {
    return _mods.at(modName);
  }
  return std::nullopt;
}
auto ModLoader::getModList() -> std::vector<std::string> {
  std::vector<std::string> modList;
  for (const auto &[modName, manifest] : _mods) {
    modList.push_back(modName);
  }
  return modList;
}
auto ModLoader::enableMod(const std::string &modName) -> bool {
  if (_mods.contains(modName)) {
    auto &mod = _mods.at(modName);
    if (mod.isEnable) {
      return true;
    }
    for (auto &parent : mod.dependencies) {
      if (!enableMod(parent)) {
        return false;
      }
    }
    _mods[modName].isEnable = true;
    return true;
  }
  return false;
}
auto ModLoader::disableMod(const std::string &modName) -> bool {
  if (_mods.contains(modName)) {
    auto &mod = _mods.at(modName);
    if (!mod.isEnable) {
      return true;
    }
    for (auto &child : mod.children) {
      disableMod(child);
    }
    mod.isEnable = false;
    return true;
  }
  return false;
}