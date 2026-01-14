#ifndef _H_CORE_RUNTIME_ASSET_
#define _H_CORE_RUNTIME_ASSET_
#include "core/Buffer.hpp"
#include "core/Instance.hpp"
#include <filesystem>
#include <memory>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace cube::runtime {
class Asset : public core::Instance {
private:
  std::unordered_map<std::string, std::string> _domains;
  mutable std::shared_mutex _mutex;

public:
  auto reset() -> void;
  auto setDomain(const std::string &name, const std::string &path) -> void;
  auto getDomain(const std::string &name,
                 const std::string &def = std::filesystem::current_path()) const
      -> const std::string &;
  auto resolve(const std::string &domain, const std::string &path) const
      -> std::string;
  auto resolve(const std::string &id) const -> std::string;
  auto load(const std::string &id) -> std::shared_ptr<core::Buffer>;
  auto load(const std::string &domain, const std::string &path) const
      -> std::shared_ptr<core::Buffer>;
  auto save(const std::string &id,
            const std::shared_ptr<core::Buffer> &data) const -> bool;
  auto save(const std::string &domain, const std::string &path,
            const std::shared_ptr<core::Buffer> &data) const -> bool;
};
} // namespace cube::runtime
#endif