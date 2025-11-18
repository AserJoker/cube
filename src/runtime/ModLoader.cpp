#include "runtime/ModLoader.hpp"
#include "core/Error.hpp"
#include "core/Json.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include "runtime/Application.hpp"
#include <algorithm>
#include <filesystem>
#include <optional>
#include <sstream>
using namespace cube;
using namespace cube::runtime;
ModLoader::ModLoader() {}
ModLoader::~ModLoader() {}
auto ModLoader::scanModList() -> void {
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  auto moddir = app.getApplicationName() + ":mods/";
  auto path = asset.resolvePath(moddir);
  auto &logger = app.getLogger();
  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  if (!std::filesystem::is_directory(path)) {
    logger.error("Mod directory {} is not a directory", moddir);
    return;
  }
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_directory()) {
      auto modName = entry.path().filename().string();
      auto manifestPath = moddir + modName + "/manifest.json";
      auto json = asset.loadAs<core::Json>(manifestPath);
      if (!json) {
        continue;
      }
      auto manifestObj = json->value;
      if (manifestObj.getType() != core::Value::Type::Object) {
        logger.error("Invalid mod manifest {} : invalid format, manifest "
                     "must be object",
                     manifestPath);
        continue;
      }
      Manifest manifest;
      auto manifestMap = manifestObj.asObject();
      if (!manifestMap->contains("name") ||
          manifestMap->at("name").getType() != core::Value::Type::String) {
        logger.error("Invalid mod manifest {} : invalid format, field "
                     "'name' is required",
                     manifestPath);
        continue;
      }
      manifest.name = manifestMap->at("name").asString().value();
      if (!manifestMap->contains("version") ||
          manifestMap->at("version").getType() != core::Value::Type::String) {
        logger.error("Invalid mod manifest {} : invalid format, field "
                     "'version' is required",
                     manifestPath);
        continue;
      }
      std::string version = manifestMap->at("version").asString().value();
      auto parsedVersion = core::Version::parse(version);
      if (!parsedVersion) {
        logger.error("Invalid mod manifest {} : invalid format, "
                     "version '{}' is invalid",
                     manifestPath, version);
        continue;
      }
      if (!manifestMap->contains("gameVersion") ||
          manifestMap->at("version").getType() != core::Value::Type::String) {
        logger.error("Invalid mod manifest {} : invalid format, field "
                     "'gameVersion' is required",
                     manifestPath);
        continue;
      }
      std::string gameVersion =
          manifestMap->at("gameVersion").asString().value();
      auto parsedGameVersion = core::Version::parse(gameVersion);
      if (!parsedGameVersion) {
        logger.error("Invalid mod manifest {} : invalid format, "
                     "version '{}' is invalid",
                     manifestPath, gameVersion);
        continue;
      }
      manifest.gameVersion = parsedGameVersion.value();
      if (manifestMap->contains("locales")) {
        auto localesVal = manifestMap->at("locales").asObject();
        if (localesVal) {
          for (const auto &[key, val] : localesVal.value()) {
            auto localeStr = val.asString();
            if (localeStr) {
              manifest.locales[key] = localeStr.value();
            } else {
              logger.error("Invalid mod manifest {} : invalid format, "
                           "field 'locales' must be string map",
                           manifestPath);
              continue;
            }
          }
        }
      }
      if (manifestMap->contains("languages")) {
        auto languagesVal = manifestMap->at("languages").asObject();
        if (languagesVal) {
          for (const auto &[key, val] : languagesVal.value()) {
            auto languageStr = val.asString();
            if (languageStr) {
              manifest.languages[key] = languageStr.value();
            } else {
              logger.error("Invalid mod manifest {} : invalid format, "
                           "field 'languages' must be string map",
                           manifestPath);
              continue;
            }
          }
        }
      }
      if (manifestMap->contains("dependencies")) {
        auto dependenciesVal = manifestMap->at("dependencies").asObject();
        if (dependenciesVal) {
          for (const auto &[name, version] : dependenciesVal.value()) {
            auto versionStr = version.asString();
            if (versionStr) {
              manifest.dependencies[name] = versionStr.value();
            } else {
              logger.error("Invalid mod manifest {} : invalid format, "
                           "field 'dependencies' must be string map",
                           manifestPath);
              continue;
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
      manifest.path = asset.resolvePath(moddir + modName + "/");
      if (_mods.contains(manifest.name)) {
        throw core::Error("Duplicate mod found : {}", manifest.name);
      }
      _mods[manifest.name] = manifest;
    }
  }
}
auto ModLoader::loadAllMods() -> void {
  std::vector<std::string> dependences;
  for (const auto &[modName, manifest] : _mods) {
    if (manifest.isEnable) {
      loadMod(modName, dependences);
    }
  }
}
auto ModLoader::loadMod(const std::string &modName,
                        std::vector<std::string> &dependences) -> void {
  auto &app = Application::getInstance();
  auto &locale = app.getLocale();
  auto &asset = app.getAsset();
  if (std::find(dependences.begin(), dependences.end(), modName) !=
      dependences.end()) {
    std::stringstream ss;
    ss << "cycle dependence found: ";
    for (auto &dep : dependences) {
      ss << dep << "->";
    }
    ss << modName;
    throw core::Error("{}", ss.str());
  }
  auto &mod = _mods.at(modName);
  if (!mod.isEnable) {
    return;
  }
  if (mod.isLoaded) {
    return;
  }
  dependences.push_back(modName);
  for (auto &[name, _] : mod.dependencies) {
    loadMod(name, dependences);
  }
  dependences.pop_back();
  asset.addDomain(mod.name, mod.path);
  for (auto &[lang, name] : mod.languages) {
    locale.addLanguage(lang, name);
  }
  for (auto &[lang, source] : mod.locales) {
    locale.addLanguageSource(lang, mod.name + ":" + source);
  }
  mod.isLoaded = true;
}

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
auto ModLoader::enableMod(const std::string &modName) -> void {
  if (!_mods.contains(modName)) {
    throw core::Error("unknown mod '{}'", modName);
  }
  auto &app = Application::getInstance();
  auto &mod = _mods.at(modName);
  if (mod.isEnable) {
    return;
  }
  if (mod.gameVersion > app.getApplicationVersion()) {
    throw core::Error("require game version '{}'", mod.name,
                      core::Version::serialize(mod.gameVersion));
  }
  for (auto &[name, ver] : mod.dependencies) {
    auto version = core::Version::parse(ver);
    if (!version) {
      throw core::Error("unknown dependence '{}@{}'", name, ver);
    }
    if (!_mods.contains(name) || version > _mods.at(name).version) {
      throw core::Error("cannot find dependence '{}@{}' required by '{}'", name,
                        ver, mod.name);
    }
    enableMod(name);
  }
  mod.isEnable = true;
}
auto ModLoader::disableMod(const std::string &modName) -> void {
  if (!_mods.contains(modName)) {
    return;
  }
  auto &manifest = _mods.at(modName);
  if (!manifest.isEnable) {
    return;
  }
  for (auto &child : manifest.children) {
    disableMod(child);
  }
  manifest.isEnable = false;
}