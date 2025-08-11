#include "runtime/Locale.hpp"
#include "core/Buffer.hpp"
#include "runtime/AssetManager.hpp"
#include <set>
#include <sstream>
#include <vector>
namespace cube::runtime {
const std::string Locale::i18n(const std::string &key) {
  if (_locales.contains(key)) {
    return _locales.at(key);
  }
  if (_defaults.contains(key)) {
    return _locales.at(key);
  }
  return key;
}
std::vector<std::string> Locale::getLanguages() {
  auto langs = _asset->queryAll("lang");
  std::set<std::string> languages;
  for (auto &lang : langs) {
    languages.insert(lang.name);
  }
  std::vector<std::string> result(languages.begin(), languages.end());
  std::sort(result.begin(), result.end());
  return result;
}

void Locale::parse(std::unordered_map<std::string, std::string> &locale,
                   const std::string &source) {
  std::stringstream ss(source.c_str());
  std::string line;
  while (std::getline(ss, line)) {
    size_t comment_pos = line.find('#');
    if (comment_pos != std::string::npos) {
      line = line.substr(0, comment_pos);
    }
    for (auto &ch : line) {
      if (ch == '\t') {
        ch = ' ';
      }
    }
    line.erase(0, line.find_first_not_of(" "));
    line.erase(line.find_last_not_of(" ") + 1);
    if (line.empty()) {
      continue;
    }
    size_t equal_pos = line.find('=');
    if (equal_pos != std::string::npos) {
      std::string key = line.substr(0, equal_pos);
      std::string value = line.substr(equal_pos + 1);
      while (key.back() == ' ') {
        key.pop_back();
      }
      while (value.front() == ' ') {
        value.erase(value.begin());
      }
      if (value.front() == '\"') {
        value = value.substr(1, value.length() - 2);
      }
      locale[key] = value;
    }
  }
}
void Locale::setLanguage(const std::string &lang) {
  _locales.clear();
  auto buf = _asset->query(std::format("lang:cube.i18n.{}", lang))
                 ->cast<core::Buffer>();
  if (!buf) {
    if (lang == "en_US") {
      return;
    }
    setLanguage("en_US");
  }
  parse(_locales, (const char *)buf->getData());
  _language = lang;
}
const std::string &Locale::getLanguage() const { return _language; }
void Locale::setDefault(const std::string &name) {
  _defaults.clear();
  auto buf = _asset->query(std::format("lang:cube.i18n.{}", name))
                 ->cast<core::Buffer>();
  if (!buf) {
    if (name == "en_US") {
      return;
    }
    setLanguage("en_US");
  }
  parse(_defaults, (const char *)buf->getData());
  _default = name;
}
const std::string &Locale::getDefault() const { return _default; }
}; // namespace cube::runtime