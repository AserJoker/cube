#pragma once
#include "core/Object.hpp"
#include "core/Value.hpp"
namespace cube::runtime {
class Config : public core::Object {
public:
  core::Value load(const std::string &domain,
                   const std::string &filename) const;
  bool save(const std::string &domain, const std::string &filename,
            const core::Value &value) const;
};
} // namespace cube::runtime