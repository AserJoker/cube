#ifndef _H_CUBE_RUNTIME_SCRIPT_
#define _H_CUBE_RUNTIME_SCRIPT_
#include "core/Instance.hpp"
#include "neo.js/core/allocator.h"
#include "neo.js/engine/runtime.h"
#include <neo.js/engine/context.h>
#include <string>

namespace cube::runtime {
class Script : public core::Instance {
private:
  neo_allocator_t _allocator = nullptr;
  neo_js_runtime_t _runtime = nullptr;
  neo_js_context_t _context = nullptr;

public:
  Script();
  ~Script() override;
  auto hasTask() const -> bool;
  auto nextTask() -> void;
  auto run(const std::string &source, const std::string &file) -> void;
};
} // namespace cube::runtime
#endif