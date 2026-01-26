#include "runtime/ModLoader.hpp"
#include "core/Value.hpp"
#include "runtime/Application.hpp"
#include "runtime/Logger.hpp"
#include "runtime/Mod.hpp"
#include <filesystem>
#include <format>
#include <sstream>
#include <stdexcept>
#include <vector>
using namespace cube;
using namespace cube::runtime;

void ModLoader::load(Mod &mod, std::vector<std::string> &path) {
  auto &manifest = mod.getManifest();
  auto &app = Application::getInstance();
  auto &logger = app.getLogger("ModLoader");
  auto &asset = app.getAsset();
  auto &locale = app.getLocale();
  auto &script = app.getScript();
  if (app.getAppVersion() < manifest.engine) {
    mod.setError(
        std::format("require {} version: {}, application version is too low",
                    app.getAppName(), manifest.engine.toString()));
    logger.error("Failed to load mod '{}': {}", manifest.name, mod.getMesage());
    return;
  }
  for (auto &[name, version] : manifest.dependences) {
    if (!_mods.contains(name)) {
      mod.setError(
          std::format("dependence {}@{} is missing", name, version.toString()));
      logger.error("Failed to load mod '{}': {}", manifest.name,
                   mod.getMesage());
      return;
    }
    auto &dep = _mods.at(name);
    if (dep.getManifest().version < version) {
      mod.setError(std::format(
          "dependence {}@{} version is too low, require {}", name,
          dep.getManifest().version.toString(), version.toString()));
      logger.error("Failed to load mod '{}': {}", manifest.name,
                   mod.getMesage());
      return;
    }
    if (std::find(path.begin(), path.end(), name) != path.end()) {
      std::stringstream ss;
      ss << "cycle dependence found: ";
      for (auto &part : path) {
        ss << part << " -> ";
      }
      ss << name;
      mod.setError(ss.str());
      logger.error("Failed to load mod '{}': {}", manifest.name,
                   mod.getMesage());
      return;
    }
    if (mod.getState() == Mod::State::ENABLE) {
      path.push_back(name);
      load(mod, path);
      path.pop_back();
      if (dep.getState() == Mod::State::ERR) {
        mod.setError(std::format(
            "Failed to load mod {}@{}, dependence mod '{}' load failed",
            manifest.name, manifest.version.toString(), name));
        return;
      }
    }
  }

  for (auto &[name, version] : manifest.optionalDependences) {
    auto &dep = _mods.at(name);
    if (dep.getManifest().version < version) {
      mod.setError(std::format(
          "dependence {}@{} version is too low, require {}", name,
          dep.getManifest().version.toString(), version.toString()));
      logger.error("Failed to load mod '{}': {}", manifest.name,
                   mod.getMesage());
      return;
    }
    if (std::find(path.begin(), path.end(), name) != path.end()) {
      std::stringstream ss;
      ss << "cycle dependence found: ";
      for (auto &part : path) {
        ss << part << " -> ";
      }
      ss << name;
      mod.setError(ss.str());
      logger.error("Failed to load mod '{}': {}", manifest.name,
                   mod.getMesage());
      return;
    }
    if (dep.getState() == Mod::State::ENABLE) {
      path.push_back(name);
      load(dep, path);
      path.pop_back();
      if (dep.getState() == Mod::State::ERR) {
        mod.setError(std::format(
            "Failed to load mod {}@{}, dependence mod '{}' load failed",
            manifest.name, manifest.version.toString(), name));
        return;
      }
    }
  }
  asset.setDomain(mod.getManifest().name, mod.getDomain());
  for (auto &[code, name] : manifest.languages) {
    locale.addLanguage(code, name);
  }
  if (manifest.preload.contains("locale")) {
    auto &modlocale = manifest.preload.at("locale");
    for (auto &[lang, src] : modlocale) {
      locale.addLanguageSource(lang, src);
    }
  }
  std::string scriptPath = asset.resolve(manifest.name, "script/index.js");
  if (std::filesystem::exists(scriptPath) &&
      !std::filesystem::is_directory(scriptPath)) {
    auto buf = asset.load(manifest.name, "script/index.js");
    script.run(std::string{(const char *)buf->getData(), buf->getSize()},
               scriptPath);
  }
  mod.ready();
}
auto ModLoader::reset() -> void { _mods.clear(); }
auto ModLoader::scanMods() -> void {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto &logger = app.getLogger("ModLoader");
  auto modpath = asset.resolve(app.getAppName(), "mods");
  if (std::filesystem::exists(modpath) &&
      std::filesystem::is_directory(modpath)) {
    for (auto &item : std::filesystem::directory_iterator(modpath)) {
      if (!item.is_directory()) {
        continue;
      }
      std::filesystem::path manifestPath = item.path() / "manifest.json";
      if (!std::filesystem::exists(manifestPath)) {
        continue;
      }
      auto buf = asset.load(app.getAppName(),
                            ("mods" / item.path().filename() / "manifest.json").string());
      if (!buf) {
        continue;
      }
      std::string src = {(const char *)buf->getData(), buf->getSize()};
      auto val = core::Value::fromJSON(src);
      if (!val) {
        logger.error("Failed to resolve manifest: {}", manifestPath.string());
        continue;
      }
      Mod::Manifest manifest{*val};
      if (_mods.contains(manifest.name)) {
        throw std::runtime_error(
            std::format("Duplicate mod found: {}@{} and {}@{}", manifest.name,
                        _mods[manifest.name].getManifest().version.toString(),
                        manifest.name, manifest.version.toString()));
      }
      _mods[manifest.name] = Mod{manifest, item.path().string()};
    }
  }
}
auto ModLoader::getMod(const std::string &name) const -> const Mod * {
  if (_mods.contains(name)) {
    return &_mods.at(name);
  }
  return nullptr;
}
auto ModLoader::getMod(const std::string &name) -> Mod * {
  if (_mods.contains(name)) {
    return &_mods.at(name);
  }
  return nullptr;
}

auto ModLoader::getMods() const
    -> const std::unordered_map<std::string, Mod> & {
  return _mods;
}

auto ModLoader::loadEnableMods() -> void {
  std::vector<std::string> path;
  for (auto &[_, mod] : _mods) {
    if (mod.getState() == Mod::State::ENABLE) {
      path.push_back(mod.getManifest().name);
      load(mod, path);
      path.pop_back();
    }
  }
}
auto ModLoader::enableMod(const std::string &name) -> void {
  if (_mods.contains(name)) {
    auto &mod = _mods.at(name);
    if (mod.getState() != Mod::State::ENABLE &&
        mod.getState() != Mod::State::READY) {
      mod.enable();
    }
    auto &manifest = mod.getManifest();
    for (auto &[dep, _] : manifest.dependences) {
      enableMod(dep);
    }
    for (auto &[dep, _] : manifest.optionalDependences) {
      enableMod(dep);
    }
  }
}
auto ModLoader::disableMod(const std::string &name) -> void {
  if (_mods.contains(name)) {
    auto &mod = _mods.at(name);
    if (mod.getState() != Mod::State::DISABLE) {
      mod.disable();
    }
    for (auto &[modname, mod] : _mods) {
      auto &manifest = mod.getManifest();
      if (manifest.dependences.contains(name)) {
        disableMod(modname);
      }
    }
  }
}