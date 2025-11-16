#pragma once
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include <memory>
#include <string>
#include <unordered_map>
namespace cube::runtime {
class Loader : public core::Object {
private:
  std::unordered_map<std::string, std::string> _domains;

public:
  auto resolvePath(const std::string &path) const -> std::string;
  auto addDomain(const std::string &name, const std::string &path) -> bool;
  auto load(const std::string &fullpath) const -> std::shared_ptr<core::Buffer>;
  template <class T>
  auto loadAs(const std::string &fullpath) const -> std::shared_ptr<T> {
    auto buffer = load(fullpath);
    if (!buffer) {
      return nullptr;
    }
    return std::make_shared<T>(buffer);
  }
};
} // namespace cube::runtime