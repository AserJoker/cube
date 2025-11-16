#pragma once
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include "core/Value.hpp"
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class Save : public core::Object {
private:
  std::string _name;
  std::string _version;
  std::vector<std::string> _mods;
  std::unordered_map<std::string, std::string> _datas;
  core::Value _config;

public:
  Save(const std::shared_ptr<core::Buffer> &buffer);
  Save(const std::string &name);
  ~Save() override;
  auto getName() const -> const std::string &;
  auto getVersion() const -> const std::string &;
  auto getMods() const -> const std::vector<std::string> &;
  auto getData(const std::string &key) const -> std::optional<std::string>;
  auto getConfig() const -> const core::Value &;
  auto save() const -> bool;

public:
  static std::shared_ptr<Save> create(const std::string &name);
};
} // namespace cube::runtime