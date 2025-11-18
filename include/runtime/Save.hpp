#pragma once
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include <memory>
#include <string>
#include <vector>
namespace cube::runtime {
class Save : public core::Object {
private:
  std::string _name;
  core::Version _version;
  std::vector<std::string> _mods;
  core::Value _config;

public:
  Save(const std::shared_ptr<core::Buffer> &buffer,
       const std::string &fullpath);
  Save(const std::string &name);
  ~Save() override;
  auto getName() const -> const std::string &;
  auto getVersion() const -> const core::Version &;
  auto getMods() const -> const std::vector<std::string> &;
  auto getConfig() const -> const core::Value &;
  auto saveManifest() const -> bool;
  auto save(const std::string &filename,
            const std::shared_ptr<core::Buffer> &buffer) -> bool;
  auto load(const std::string &filename) -> std::shared_ptr<core::Buffer>;

public:
  static std::shared_ptr<Save> create(const std::string &name);
};
} // namespace cube::runtime