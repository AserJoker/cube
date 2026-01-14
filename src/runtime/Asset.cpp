#include "runtime/Asset.hpp"
#include "core/Buffer.hpp"
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
using namespace cube;
using namespace cube::runtime;
auto Asset::setDomain(const std::string &name, const std::string &path)
    -> void {
  std::unique_lock<std::shared_mutex> lock(_mutex);
  _domains[name] = path;
}
auto Asset::reset() -> void {
  std::unique_lock<std::shared_mutex> lock(_mutex);
  _domains.clear();
}
auto Asset::getDomain(const std::string &name, const std::string &def) const
    -> const std::string & {
  std::shared_lock<std::shared_mutex> lock(_mutex);
  if (_domains.contains(name)) {
    return _domains.at(name);
  }
  return def;
}
auto Asset::resolve(const std::string &domain, const std::string &path) const
    -> std::string {
  if (domain.empty()) {
    return path;
  }
  std::filesystem::path dpath = getDomain(domain);
  dpath.append(path);
  return dpath.string();
}
auto Asset::resolve(const std::string &id) const -> std::string {
  size_t idx = id.find_first_of(':');
  if (idx == std::string::npos) {
    return id;
  }
  std::string domain = id.substr(0, idx);
  std::string path = id.substr(idx + 1);
  return resolve(domain, path);
}
auto Asset::load(const std::string &id) -> std::shared_ptr<core::Buffer> {
  size_t idx = id.find_first_of(':');
  if (idx == std::string::npos) {
    return nullptr;
  }
  std::string domain = id.substr(0, idx);
  std::string path = id.substr(idx + 1);
  return load(domain, path);
}
auto Asset::load(const std::string &domain, const std::string &p) const
    -> std::shared_ptr<core::Buffer> {
  std::string path = resolve(domain, p);
  if (!std::filesystem::exists(path)) {
    return nullptr;
  }
  if (std::filesystem::is_directory(path)) {
    return nullptr;
  }
  std::ifstream file;
  file.open(path, std::ios::binary);
  if (!file.is_open()) {
    return nullptr;
  }
  size_t len = 0;
  file.seekg(0, std::ios::end);
  len = file.tellg();
  file.seekg(0, std::ios::beg);
  std::shared_ptr<core::Buffer> buf = std::make_shared<core::Buffer>(len);
  file.read((char *)buf->getData(), len);
  return buf;
}
auto Asset::save(const std::string &id,
                 const std::shared_ptr<core::Buffer> &data) const -> bool {
  size_t idx = id.find_first_of(':');
  if (idx == std::string::npos) {
    return false;
  }
  std::string domain = id.substr(0, idx);
  std::string path = id.substr(idx + 1);
  return save(domain, path, data);
}
auto Asset::save(const std::string &domain, const std::string &p,
                 const std::shared_ptr<core::Buffer> &data) const -> bool {
  std::filesystem::path path = resolve(domain, p);
  if (!std::filesystem::exists(path.parent_path())) {
    if (!std::filesystem::create_directories(path.parent_path())) {
      return false;
    }
  }
  if (!std::filesystem::is_directory(path.parent_path())) {
    return false;
  }
  std::ofstream file;
  file.open(path, std::ios::binary);
  if (!file.is_open()) {
    return false;
  }
  file.write((const char *)data->getData(), data->getSize());
  return true;
}