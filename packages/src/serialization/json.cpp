#include "serialization/json.hpp"
#include "core/Object.hpp"
#include "reflection/ObjectType.hpp"
#include "reflection/Variable.hpp"
#include <cjson/cJSON.h>
#include <format>
#include <stdexcept>

namespace cube::serialization {
static reflection::Variable *parseCJSONObject(cJSON *object) {
  if (object->type == cJSON_NULL) {
    return core::Object::createObject<reflection::Variable>();
  }
  if (object->type == cJSON_False) {
    return core::Object::createObject<reflection::Variable>()->setBoolean(
        false);
  }
  if (object->type == cJSON_True) {
    return core::Object::createObject<reflection::Variable>()->setBoolean(true);
  }
  if (object->type == cJSON_Number) {
    return core::Object::createObject<reflection::Variable>()->setNumber(
        object->valuedouble);
  }
  if (object->type == cJSON_String) {
    return core::Object::createObject<reflection::Variable>()->setString(
        object->valuestring);
  }
  if (object->type == cJSON_Array) {
    auto variable =
        core::Object::createObject<reflection::Variable>()->setArray();
    size_t size = cJSON_GetArraySize(object);
    variable->setSize(size);
    for (size_t idx = 0; idx < size; idx++) {
      cJSON *item = cJSON_GetArrayItem(object, idx);
      variable->setIndex(idx, parseCJSONObject(item));
    }
    return variable;
  } else {
    auto variable =
        core::Object::createObject<reflection::Variable>()->setDict();
    cJSON *node = object->child;
    while (node) {
      variable->setField(node->string, parseCJSONObject(node));
      node = node->next;
    }
    return variable;
  }
}

reflection::Variable *Json::parse(const std::string &source) {
  cJSON *root = cJSON_Parse(source.c_str());
  if (!root) {
    throw std::runtime_error(cJSON_GetErrorPtr());
  }
  reflection::Variable *result = parseCJSONObject(root);
  cJSON_free(root);
  return result;
}

static std::string stringifyDict(reflection::Variable *variable) {
  std::string result = "{";
  auto keys = variable->getKeys();
  size_t idx = 0;
  for (auto &key : keys) {
    if (idx != 0) {
      result += ',';
    }
    result += '"' + key + '"' + ':' + Json::stringify(variable->getField(key));
    idx++;
  }
  result += "}";
  return result;
}
static std::string stringifyArray(reflection::Variable *variable) {
  std::string result = "[";
  size_t idx = 0;
  size_t size = variable->getSize();
  for (idx = 0; idx < size; idx++) {
    if (idx != 0) {
      result += ',';
    }
    result += Json::stringify(variable->getIndex(idx));
  }
  result += "]";
  return result;
}
static std::string stringifyObject(reflection::Variable *variable) {
  core::Object defer;
  auto *type = variable->getType()->cast<reflection::ObjectType>();
  if (type->hasMethod("serialize")) {
    return Json::stringify(type->invoke(variable, "serialize")
                               ->setParent(variable)
                               ->cast<reflection::Variable>());
  }
  std::string result = "{";
  auto keys = type->getFields();
  size_t idx = 0;
  for (auto &key : keys) {
    if (idx != 0) {
      result += ',';
    }
    result +=
        '"' + key + '"' + ':' + Json::stringify(type->getField(variable, key));
    idx++;
  }
  result += "}";
  return result;
}

std::string Json::stringify(reflection::Variable *variable) {
  switch (variable->getType()->getKind()) {
  case reflection::Type::Kind::NIL:
    return "null";
  case reflection::Type::Kind::NUMBER:
    return std::to_string(variable->getNumber());
  case reflection::Type::Kind::INTEGER:
    return std::to_string(variable->getInteger());
  case reflection::Type::Kind::STRING:
    return std::format("\"{}\"", variable->getString());
  case reflection::Type::Kind::BOOLEAN:
    return variable->getBoolean() ? "true" : "false";
  case reflection::Type::Kind::DICT:
    return stringifyDict(variable);
  case reflection::Type::Kind::ARRAY:
    return stringifyArray(variable);
  case reflection::Type::Kind::OBJECT:
    return stringifyObject(variable);
  }
  return "";
}
}; // namespace cube::serialization