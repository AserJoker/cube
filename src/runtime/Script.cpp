#include "runtime/Script.hpp"
#include "neo.js/core/allocator.h"
#include "neo.js/core/string.h"
#include "neo.js/engine/exception.h"
#include "neo.js/engine/runtime.h"
#include "neo.js/engine/string.h"
#include "neo.js/engine/symbol.h"
#include "runtime/Application.hpp"
using namespace cube;
using namespace cube::runtime;

void JSErrorCallback(neo_js_context_t ctx, neo_js_variable_t error) {
  auto &app = Application::getInstance();
  auto &logger = app.getLogger("Script");
  neo_allocator_t allocator =
      neo_js_runtime_get_allocator(neo_js_context_get_runtime(ctx));
  if (error->value->type == NEO_JS_TYPE_SYMBOL) {
    neo_js_symbol_t symbol = (neo_js_symbol_t)error->value;
    char *description = neo_string16_to_string(allocator, symbol->description);
    logger.error("Uncaught Symbol({})\n", description);
    neo_allocator_free(allocator, description);
    return;
  } else if (error->value->type != NEO_JS_TYPE_STRING) {
    error = neo_js_variable_to_string(error, ctx);
    if (error->value->type == NEO_JS_TYPE_EXCEPTION) {
      neo_js_exception_t exception = (neo_js_exception_t)error->value;
      error = neo_js_context_create_variable(ctx, exception->error);
      return JSErrorCallback(ctx, error);
    }
  }
  const uint16_t *err = ((neo_js_string_t)error->value)->value;
  char *msg = neo_string16_to_string(allocator, err);
  logger.error("Uncaught {}\n", msg);
  neo_allocator_free(allocator, msg);
}

Script::Script() {
  _allocator = neo_create_allocator(NULL);
  _runtime = neo_create_js_runtime(_allocator);
  _context = neo_create_js_context(_runtime);
  neo_js_context_set_error_callback(_context, JSErrorCallback);
}

Script::~Script() {
  neo_allocator_free(_allocator, _context);
  neo_allocator_free(_allocator, _runtime);
  neo_delete_allocator(_allocator);
}

auto Script::hasTask() const -> bool {
  return neo_js_context_has_task(_context);
}

auto Script::nextTask() -> void { neo_js_context_next_task(_context); }

auto Script::run(const std::string &source, const std::string &file) -> void {
  neo_js_context_eval(_context, source.c_str(), file.c_str(),
                      NEO_JS_EVAL_MODULE);
}