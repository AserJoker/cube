#include "runtime/Locale.hpp"
#include "runtime/Application.hpp"
#include <string>
#include <unordered_map>
#include <vector>
using namespace cube;
using namespace cube::runtime;

auto Locale::load(const std::string &id,
                  std::unordered_map<std::string, std::string> &locales) const
    -> void {
  auto &asset = Application::getInstance().getAsset();
  auto &logger = Application::getInstance().getLogger("Locale");
  if (!_languages.contains(id)) {
    logger.error("Unknown language code '{}'", id);
    return;
  }
  for (auto &source : getLanguageSource(id)) {
    auto buf = asset.load(source);
    if (!buf) {
      logger.warn("Failed to load locale file '{}' for '{}'", source, id);
      continue;
    }
    std::string src{(const char *)buf->getData(), buf->getSize()};
    size_t idx = 0;
    while (!src.empty()) {
      idx = src.find_first_of('\n');
      std::string line;
      if (idx != std::string::npos) {
        line = src.substr(0, idx);
        src = src.substr(idx + 1);
      } else {
        line = src;
        src.clear();
      }
      idx = line.find_first_of('#');
      if (idx != std::string::npos) {
        line = line.substr(0, idx);
      }
      std::string key;
      std::string value;
      idx = line.find_first_of('=');
      if (idx != std::string::npos) {
        key = line.substr(0, idx);
        value = line.substr(idx + 1);

        idx = key.find_first_not_of(" \t\r\f\v");
        if (idx != std::string::npos) {
          key = key.substr(idx);
        }
        idx = key.find_last_not_of(" \t\r\f\v");
        if (idx != std::string::npos) {
          key = key.substr(0, idx + 1);
        }

        idx = value.find_first_not_of(" \t\r\f\v");
        if (idx != std::string::npos) {
          value = value.substr(idx);
        }
        idx = value.find_last_not_of(" \t\r\f\v");
        if (idx != std::string::npos) {
          value = value.substr(0, idx + 1);
        }
        if (value.starts_with('\"')) {
          value = value.substr(1);
        }
        if (value.ends_with('\"')) {
          value = value.substr(0, value.length() - 1);
        }
        locales[key] = value;
      }
    }
  }
}
auto Locale::reset() -> void {
  _locales.clear();
  _defs.clear();
  _languages.clear();
  _language = "";
  _defaultLanguage = "";
}
auto Locale::getLanguage() const -> const std::string & { return _language; }
auto Locale::getDefaultLanguage() const -> const std::string & {
  return _defaultLanguage;
}
auto Locale::getLanguages() const
    -> const std::unordered_map<std::string, std::string> & {
  return _languages;
}
auto Locale::getLanguageSource(const std::string &id) const
    -> const std::vector<std::string> & {
  static std::vector<std::string> empty = {};
  if (_sources.contains(id)) {
    return _sources.at(id);
  }
  return empty;
}
auto Locale::addLanguageSource(const std::string &id, const std::string &source)
    -> void {
  auto &sources = _sources[id];
  sources.push_back(source);
}
auto Locale::addLanguage(const std::string &id, const std::string &name)
    -> void {
  _languages[id] = name;
}
auto Locale::setDefaultLanguage(const std::string &id) -> void {
  _defs.clear();
  load(id, _defs);
}
auto Locale::setLanguage(const std::string &id) -> void {
  _locales.clear();
  load(id, _locales);
}

auto Locale::i18n(const std::string &key, const std::string &def) const
    -> const std::string & {
  if (_locales.contains(key)) {
    return _locales.at(key);
  }
  if (_defs.contains(key)) {
    return _defs.at(key);
  }
  if (!def.empty()) {
    return def;
  }
  return key;
}
auto Locale::i18n(const std::string &key,
                  const std::unordered_map<std::string, std::string> &params,
                  const std::string &def) const -> std::string {
  std::string result = i18n(key, def);
  for (auto &[field, value] : params) {
    std::string fmt = "{" + field + "}";
    size_t idx = result.find_first_of(field);
    while (idx != std::string::npos) {
      result = result.replace(idx, field.length(), value);
      idx = result.find_first_of(field);
    }
  }
  return result;
}