#pragma once
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include "runtime/AssetManager.hpp"
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class Locale : public core::Object {
private:
  AssetManager *_asset = core::Singleton<AssetManager>::get();

private:
  std::unordered_map<std::string, std::string> _locales;
  std::string _language;

public:
  const std::string i18n(const std::string &key);
  std::vector<std::string> getLanguages();
  void setLanguage(const std::string &name);
  const std::string &getLanguage() const;
};
}; // namespace cube::runtime