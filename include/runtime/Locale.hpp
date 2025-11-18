#pragma once
#include "core/Object.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class Locale : public core::Object {
public:
  using Parameters = std::unordered_map<std::string, std::string>;

private:
  std::string _language;
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::vector<std::string>> _languages;
  std::unordered_map<std::string, std::string> _languageNames;

public:
  auto addLanguage(const std::string &lang, const std::string &name) -> void;
  auto addLanguageSource(const std::string &lang, const std::string &source)
      -> void;
  auto setLang(const std::string &lang) -> bool;
  auto getLang() const -> const std::string &;
  auto i18n(const std::string &key, const std::string &def = "",
            const Parameters &params = {}) const -> std::string;
  auto getLanguages() const
      -> const std::unordered_map<std::string, std::string> &;
};
} // namespace cube::runtime