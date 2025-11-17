#include "runtime/Locale.hpp"
#include "runtime/Application.hpp"
#include <algorithm>
using namespace cube;
using namespace cube::runtime;
auto Locale::setLang(const std::string &lang) -> bool {
  if (!_languages.contains(lang)) {
    return false;
  }
  _language = lang;
  _locales.clear();
  auto &app = Application::getInstance();
  auto &asset = app.getAsset();
  for (const auto &source : _languages.at(lang)) {
    auto buffer = asset.load(source);
    if (!buffer) {
      continue;
    }
    std::string content(static_cast<const char *>(buffer->getData()),
                        buffer->getSize());
    size_t pos = 0;
    while (pos < content.size()) {
      size_t endLine = content.find('\n', pos);
      if (endLine == std::string::npos) {
        endLine = content.size();
      }
      std::string line = content.substr(pos, endLine - pos);
      size_t commentPos = line.find('#');
      if (commentPos != std::string::npos) {
        line = line.substr(0, commentPos);
      }
      size_t sepPos = line.find('=');
      if (sepPos != std::string::npos) {
        std::string key = line.substr(0, sepPos);
        std::string value = line.substr(sepPos + 1);
        key.erase(0, key.find_first_not_of(" \t\r"));
        key.erase(key.find_last_not_of(" \t\r") + 1);
        value.erase(0, value.find_first_not_of(" \t\r"));
        value.erase(value.find_last_not_of(" \t\r") + 1);
        if (value.size() >= 2 && value.front() == '\"' &&
            value.back() == '\"') {
          value = value.substr(1, value.size() - 2);
        }
        _locales[key] = value;
      }
      pos = endLine + 1;
    }
  }
  return true;
}

auto Locale::addLanguage(const std::string &lang, const std::string &name)
    -> void {
  _languageNames[lang] = name;
}
auto Locale::addLanguageSource(const std::string &lang,
                               const std::string &source) -> void {
  auto &sources = _languages[lang];
  if (std::find(sources.begin(), sources.end(), source) == sources.end()) {
    sources.push_back(source);
  }
}

auto Locale::getLang() const -> const std::string & { return _language; }

auto Locale::i18n(const std::string &key, const std::string &def,
                  const Parameters &params) const -> std::string {
  auto it = _locales.find(key);
  std::string result = (it != _locales.end()) ? it->second : def;
  if (result.empty()) {
    result = key;
  }
  for (const auto &[paramKey, paramValue] : params) {
    std::string placeholder = "{" + paramKey + "}";
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
      result.replace(pos, placeholder.length(), paramValue);
      pos += paramValue.length();
    }
  }
  return result;
}