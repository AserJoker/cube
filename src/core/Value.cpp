#include "core/Value.hpp"
#include <any>
#include <cjson/cJSON.h>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace cube;
using namespace cube::core;

auto Value::getType() const -> const Type & { return _type; }
auto Value::setNil() -> Value & {
  _value.reset();
  _type = Type::NIL;
  return *this;
}
auto Value::setNumber(double value) -> Value & {
  _value = value;
  _type = Type::NUMBER;
  return *this;
}
auto Value::setBoolean(bool value) -> Value & {
  _value = value;
  _type = Type::BOOLEAN;
  return *this;
}
auto Value::setString(const std::string &value) -> Value & {
  _value = value;
  _type = Type::STRING;
  return *this;
}
auto Value::setArray() -> Value & {
  _value = std::vector<Value>{};
  _type = Type::ARRAY;
  return *this;
}
auto Value::setObject() -> Value & {
  _value = std::unordered_map<std::string, Value>{};
  _type = Type::OBJECT;
  return *this;
}
auto Value::isNil() const -> bool { return _type == Type::NIL; }

auto Value::isNumber() const -> bool { return _type == Type::NUMBER; }
auto Value::isString() const -> bool { return _type == Type::STRING; }
auto Value::isBoolean() const -> bool { return _type == Type::BOOLEAN; }
auto Value::isArray() const -> bool { return _type == Type::ARRAY; }
auto Value::isObject() const -> bool { return _type == Type::OBJECT; }
auto Value::asNumber() const -> const double * {
  return std::any_cast<const double>(&_value);
}
auto Value::asString() const -> const std::string * {
  return std::any_cast<const std::string>(&_value);
}
auto Value::asBoolean() const -> const bool * {
  return std::any_cast<const bool>(&_value);
}
auto Value::asArray() const -> const std::vector<Value> * {
  return std::any_cast<const std::vector<Value>>(&_value);
}
auto Value::asObject() const -> const std::unordered_map<std::string, Value> * {
  return std::any_cast<const std::unordered_map<std::string, Value>>(&_value);
}

auto Value::asNumber() -> double * { return std::any_cast<double>(&_value); }
auto Value::asString() -> std::string * {
  return std::any_cast<std::string>(&_value);
}
auto Value::asBoolean() -> bool * { return std::any_cast<bool>(&_value); }
auto Value::asArray() -> std::vector<Value> * {
  return std::any_cast<std::vector<Value>>(&_value);
}
auto Value::asObject() -> std::unordered_map<std::string, Value> * {
  return std::any_cast<std::unordered_map<std::string, Value>>(&_value);
}

auto Value::getNumber(double val) const -> double {
  if (_type == Type::NUMBER) {
    return std::any_cast<const double &>(_value);
  }
  return val;
}
auto Value::getString(const std::string &val) const -> std::string {
  if (_type == Type::STRING) {
    return std::any_cast<const std::string &>(_value);
  }
  return val;
}
auto Value::getBoolean(bool val) const -> bool {
  if (_type == Type::BOOLEAN) {
    return std::any_cast<bool>(_value);
  }
  return val;
}
auto Value::getArray(const std::vector<Value> &val) const
    -> std::vector<Value> {
  if (_type == Type::ARRAY) {
    return std::any_cast<const std::vector<Value> &>(_value);
  }
  return val;
}
auto Value::getObject(const std::unordered_map<std::string, Value> &val) const
    -> std::unordered_map<std::string, Value> {
  if (_type == Type::OBJECT) {
    return std::any_cast<const std::unordered_map<std::string, Value> &>(
        _value);
  }
  return val;
}

auto Value::getField(const std::string &key, const Value &val) const
    -> const Value {
  if (_type == Type::OBJECT) {
    auto obj = getObject();
    if (obj.contains(key)) {
      return obj.at(key);
    }
  }
  return val;
}
auto Value::getIndex(size_t idx, const Value &val) const -> const Value {
  if (_type == Type::ARRAY) {
    auto arr = getArray();
    if (arr.size() > idx) {
      return arr.at(idx);
    }
  }
  return val;
}
auto Value::getLength() const -> size_t {
  if (_type == Type::ARRAY) {
    auto arr = getArray();
    return arr.size();
  }
  return 0;
}

static cJSON *toJSONNode(const Value &value) {
  cJSON *node = NULL;
  switch (value.getType()) {
  case Value::Type::NIL:
    node = cJSON_CreateNull();
    break;
  case Value::Type::NUMBER:
    node = cJSON_CreateNumber(*value.asNumber());
    break;
  case Value::Type::STRING:
    node = cJSON_CreateString(value.asString()->c_str());
    break;
  case Value::Type::BOOLEAN:
    node = cJSON_CreateBool(*value.asBoolean());
    break;
  case Value::Type::ARRAY: {
    node = cJSON_CreateArray();
    auto &arr = *value.asArray();
    for (auto &item : arr) {
      auto itemNode = toJSONNode(item);
      cJSON_AddItemToArray(node, itemNode);
    }
    break;
  }
  case Value::Type::OBJECT: {
    node = cJSON_CreateObject();
    auto &obj = *value.asObject();
    for (auto &[key, value] : obj) {
      auto valueNode = toJSONNode(value);
      cJSON_AddItemToObject(node, key.c_str(), valueNode);
    }
    break;
  }
  }
  return node;
}

auto Value::toJSON() const -> std::string {
  cJSON *node = toJSONNode(*this);
  char *result = cJSON_PrintUnformatted(node);
  cJSON_free(node);
  std::string res = result;
  cJSON_free(result);
  return res;
}

static void parseJSONNode(cJSON *node, Value &value) {
  if (cJSON_IsNull(node)) {
    value.setNil();
    return;
  }
  if (cJSON_IsNumber(node)) {
    value.setNumber(node->valuedouble);
    return;
  }
  if (cJSON_IsBool(node)) {
    value.setBoolean(node->valueint);
    return;
  }
  if (cJSON_IsString(node)) {
    value.setString(node->valuestring);
    return;
  }
  if (cJSON_IsArray(node)) {
    size_t len = cJSON_GetArraySize(node);
    auto &arr = *value.setArray().asArray();
    size_t idx = 0;
    while (idx < len) {
      auto itemNode = cJSON_GetArrayItem(node, idx);
      arr.push_back({});
      parseJSONNode(itemNode, arr.back());
      idx++;
    }
    return;
  }
  if (cJSON_IsObject(node)) {
    cJSON *child = node->child;
    auto &obj = *value.setObject().asObject();
    while (child) {
      parseJSONNode(child, obj[child->string]);
      child = child->next;
    }
    return;
  }
}

std::shared_ptr<Value> Value::fromJSON(const std::string &src) {
  cJSON *node = cJSON_Parse(src.c_str());
  if (!node) {
    return nullptr;
  }
  auto value = std::make_shared<Value>();
  parseJSONNode(node, *value);
  cJSON_free(node);
  return value;
}