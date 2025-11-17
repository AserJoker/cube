#pragma once
#include "core/Buffer.hpp"
#include "core/Value.hpp"
#include <memory>
namespace cube::core {
struct Json {
  core::Value value;
  Json(const std::shared_ptr<Buffer> &buffer, const std::string &filename);
};
} // namespace cube::core