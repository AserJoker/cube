#include "core/Json.hpp"
#include "core/Buffer.hpp"
#include "core/Error.hpp"
#include <cjson/cJSON.h>
#include <format>
#include <memory>
using namespace cube;
using namespace cube::core;
static auto resolveJSONError(const std::string &source, size_t errpos,
                             const std::string &filename) -> std::string {
  size_t pos = 0;
  size_t line = 0;
  size_t column = 0;
  auto next = 0;
  while (pos < source.length()) {
    next = source.find_first_of('\n', pos);
    if (next > errpos) {
      column = errpos - pos;
      break;
    }
    line++;
    pos = next + 1;
  }
  return std::format("Failed to resolve {}:{}:{}\n{}", filename, line + 1,
                     column, source.substr(pos, next - pos));
}
Json::Json(const std::shared_ptr<Buffer> &buffer, const std::string &filename) {
  std::string content(static_cast<const char *>(buffer->getData()),
                      buffer->getSize());
  auto source = content.c_str();
  auto node = cJSON_Parse(source);
  if (!node) {
    auto error = cJSON_GetErrorPtr();
    auto errpos = error - source;
    throw core::Error("{}", resolveJSONError(content, errpos, filename));
  }
  value = core::Value::parseJSON(node);
}