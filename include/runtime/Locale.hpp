#ifndef _H_CUBE_RUNTIME_LOCALE_
#define _H_CUBE_RUNTIME_LOCALE_
#include "core/Instance.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class Locale : public core::Instance {
private:
  std::unordered_map<std::string, std::string> _languages;
  std::unordered_map<std::string, std::vector<std::string>> _sources;
  std::unordered_map<std::string, std::string> _locales;
  std::unordered_map<std::string, std::string> _defs;
  std::string _language;
  std::string _defaultLanguage;

private:
  auto load(const std::string &id,
            std::unordered_map<std::string, std::string> &locale) const -> void;

public:
  auto reset() -> void;
  auto getLanguage() const -> const std::string &;
  auto getDefaultLanguage() const -> const std::string &;
  auto getLanguages() const
      -> const std::unordered_map<std::string, std::string> &;
  auto getLanguageSource(const std::string &id) const
      -> const std::vector<std::string> &;
  auto addLanguageSource(const std::string &id, const std::string &source)
      -> void;
  auto addLanguage(const std::string &id, const std::string &name) -> void;
  auto setDefaultLanguage(const std::string &id) -> void;
  auto setLanguage(const std::string &id) -> void;
  auto i18n(const std::string &key, const std::string &def = "") const
      -> const std::string &;
  auto i18n(const std::string &key,
            const std::unordered_map<std::string, std::string> &params,
            const std::string &def = "") const -> std::string;
};
} // namespace cube::runtime
#endif