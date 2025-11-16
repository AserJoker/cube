#include "core/Value.hpp"
using namespace cube;
using namespace cube::core;
Value::Value() : Object(), _node(std::make_shared<NullNode>()) {}

Value::~Value() = default;

auto Value::createNull() -> Value {
  Value v;
  v._node = std::make_shared<NullNode>();
  return v;
}

auto Value::createBoolean(bool v) -> Value {
  Value val;
  val._node = std::make_shared<BooleanNode>(v);
  return val;
}

auto Value::createInteger(int64_t v) -> Value {
  Value val;
  val._node = std::make_shared<IntegerNode>(v);
  return val;
}

auto Value::createFloat(double v) -> Value {
  Value val;
  val._node = std::make_shared<FloatNode>(v);
  return val;
}

auto Value::createString(const std::string &v) -> Value {
  Value val;
  val._node = std::make_shared<StringNode>(v);
  return val;
}

auto Value::createArray() -> Value {
  Value val;
  val._node = std::make_shared<ArrayNode>();
  return val;
}

auto Value::createObject() -> Value {
  Value val;
  val._node = std::make_shared<ObjectNode>();
  return val;
}

auto Value::asBoolean() const -> std::optional<bool> {
  if (_node->type != Type::Boolean) {
    return std::nullopt;
  }
  auto bnode = std::static_pointer_cast<BooleanNode>(_node);
  return bnode->value;
}

auto Value::asInteger() const -> std::optional<int64_t> {
  if (_node->type != Type::Integer) {
    return std::nullopt;
  }
  auto inode = std::static_pointer_cast<IntegerNode>(_node);
  return inode->value;
}

auto Value::asFloat() const -> std::optional<double> {
  if (_node->type != Type::Float) {
    return std::nullopt;
  }
  auto fnode = std::static_pointer_cast<FloatNode>(_node);
  return fnode->value;
}

auto Value::asString() const -> std::optional<std::string> {
  if (_node->type != Type::String) {
    return std::nullopt;
  }
  auto snode = std::static_pointer_cast<StringNode>(_node);
  return snode->value;
}

auto Value::asArray() const -> std::optional<std::vector<Value>> {
  if (_node->type != Type::Array) {
    return std::nullopt;
  }
  auto anode = std::static_pointer_cast<ArrayNode>(_node);
  std::vector<Value> result;
  for (const auto &elem : anode->values) {
    Value v;
    v._node = elem;
    result.push_back(v);
  }
  return result;
}

auto Value::asObject() const
    -> std::optional<std::unordered_map<std::string, Value>> {
  if (_node->type != Type::Object) {
    return std::nullopt;
  }
  auto onode = std::static_pointer_cast<ObjectNode>(_node);
  std::unordered_map<std::string, Value> result;
  for (const auto &[key, valNode] : onode->values) {
    Value v;
    v._node = valNode;
    result[key] = v;
  }
  return result;
}

auto Value::getType() const -> Type { return _node->type; }

auto Value::getLength() const -> std::optional<size_t> {
  if (_node->type == Type::Array) {
    auto anode = std::static_pointer_cast<ArrayNode>(_node);
    return anode->values.size();
  } else if (_node->type == Type::Object) {
    auto onode = std::static_pointer_cast<ObjectNode>(_node);
    return onode->values.size();
  }
  return std::nullopt;
}

auto Value::getKeys() const -> std::optional<std::vector<std::string>> {
  if (_node->type != Type::Object) {
    return std::nullopt;
  }
  auto onode = std::static_pointer_cast<ObjectNode>(_node);
  std::vector<std::string> keys;
  for (const auto &[key, _] : onode->values) {
    keys.push_back(key);
  }
  return keys;
}

auto Value::get(const std::string &path) const -> std::optional<Value> {
  if (_node->type != Type::Object && _node->type != Type::Array) {
    return std::nullopt;
  }
  size_t pos = 0;
  auto current = _node;
  while (pos < path.length()) {
    if (path[pos] == '[') {
      if (current->type != Type::Array) {
        return std::nullopt;
      }
      size_t endBracketPos = path.find_first_of("]", pos);
      if (endBracketPos == std::string::npos) {
        return std::nullopt;
      }
      std::string indexStr = path.substr(1, endBracketPos - 1);
      size_t index = std::stoul(indexStr);
      auto anode = std::static_pointer_cast<ArrayNode>(current);
      if (index >= anode->values.size()) {
        return std::nullopt;
      }
      current = anode->values[index];
      pos = endBracketPos + 1;
      continue;
    }
    size_t endPos = path.find_first_of("[", pos);
    if (endPos == std::string::npos) {
      endPos = path.find_first_of('.', pos);
    }
    std::string key;
    if (endPos != std::string::npos) {
      key = path.substr(pos, endPos - pos);
      pos = endPos + 1;
    } else {
      key = path.substr(pos);
      pos = path.size();
    }
    if (current->type != Type::Object) {
      return std::nullopt;
    }
    auto onode = std::static_pointer_cast<ObjectNode>(current);
    if (!onode->values.contains(key)) {
      return std::nullopt;
    }
    current = onode->values[key];
  }
  Value result;
  result._node = current;
  return result;
}

auto Value::getField(const std::string &key) const -> std::optional<Value> {
  if (_node->type != Type::Object) {
    return std::nullopt;
  }
  auto onode = std::static_pointer_cast<ObjectNode>(_node);
  if (!onode->values.contains(key)) {
    return std::nullopt;
  }
  Value val;
  val._node = onode->values[key];
  return val;
}

auto Value::getElement(size_t index) const -> std::optional<Value> {
  if (_node->type != Type::Array) {
    return std::nullopt;
  }
  auto anode = std::static_pointer_cast<ArrayNode>(_node);
  if (index >= anode->values.size()) {
    return std::nullopt;
  }
  Value val;
  val._node = anode->values[index];
  return val;
}

auto Value::setNull() -> void { _node = std::make_shared<NullNode>(); }

auto Value::setInteger(int64_t v) -> void {
  _node = std::make_shared<IntegerNode>(v);
}

auto Value::setFloat(double v) -> void {
  _node = std::make_shared<FloatNode>(v);
}

auto Value::setBoolean(bool v) -> void {
  _node = std::make_shared<BooleanNode>(v);
}

auto Value::setString(const std::string &v) -> void {
  _node = std::make_shared<StringNode>(v);
}

auto Value::setArray() -> void { _node = std::make_shared<ArrayNode>(); }

auto Value::setObject() -> void { _node = std::make_shared<ObjectNode>(); }

auto Value::setField(const std::string &key, const Value &value) -> bool {
  if (_node->type != Type::Object) {
    return false;
  }
  auto onode = std::static_pointer_cast<ObjectNode>(_node);
  onode->values[key] = value._node;
  return true;
}

auto Value::setElement(size_t index, const Value &value) -> bool {
  if (_node->type != Type::Array) {
    return false;
  }
  auto anode = std::static_pointer_cast<ArrayNode>(_node);
  if (index >= anode->values.size()) {
    return false;
  }
  anode->values[index] = value._node;
  return true;
}

auto Value::set(const std::string &path, const Value &value) -> bool {
  size_t pos = 0;
  auto current = &_node;
  while (pos < path.size()) {
    if (path[pos] == '[') {
      size_t endBracketPos = path.find_first_of("]", pos);
      if (endBracketPos == std::string::npos) {
        return false;
      }
      if (current->get()->type != Type::Array) {
        current->reset(new ArrayNode());
      }
      std::string indexStr = path.substr(pos + 1, endBracketPos - pos - 1);
      size_t index = std::stoul(indexStr);
      auto anode = std::static_pointer_cast<ArrayNode>(*current);
      if (index >= anode->values.size()) {
        anode->values.resize(index + 1);
      }
      current = &anode->values[index];
      pos = endBracketPos + 1;
      continue;
    }
    size_t endPos = path.find_first_of("[", pos);
    if (endPos == std::string::npos) {
      endPos = path.find_first_of('.', pos);
    }
    if (current->get()->type != Type::Object) {
      current->reset(new ObjectNode());
    }
    if (endPos != std::string::npos) {
      std::string key = path.substr(pos, endPos - pos);
      auto onode = std::static_pointer_cast<ObjectNode>(*current);
      if (!onode->values.contains(key)) {
        onode->values[key] = std::make_shared<NullNode>();
      }
      current = &onode->values[key];
      pos = endPos + 1;
    } else {
      std::string key = path.substr(pos);
      auto onode = std::static_pointer_cast<ObjectNode>(*current);
      onode->values[key] = value._node;
      return true;
    }
  }
  return true;
}

auto Value::setLength(size_t length) -> bool {
  if (_node->type == Type::Array) {
    auto anode = std::static_pointer_cast<ArrayNode>(_node);
    anode->values.resize(length);
    return true;
  }
  return false;
}

auto Value::removeField(const std::string &key) -> bool {
  if (_node->type != Type::Object) {
    return false;
  }
  auto onode = std::static_pointer_cast<ObjectNode>(_node);
  return onode->values.erase(key) > 0;
}

auto Value::appendElement(const Value &value) -> bool {
  if (_node->type != Type::Array) {
    return false;
  }
  auto anode = std::static_pointer_cast<ArrayNode>(_node);
  anode->values.push_back(value._node);
  return true;
}

auto Value::parseJSON(cJSON *node) -> Value {
  if (!node) {
    return Value::createNull();
  }
  switch (node->type) {
  case cJSON_False:
    return Value::createBoolean(false);
  case cJSON_True:
    return Value::createBoolean(true);
  case cJSON_NULL:
    return Value::createNull();
  case cJSON_Number:
    if (node->valuedouble == static_cast<double>(node->valueint)) {
      return Value::createInteger(node->valueint);
    } else {
      return Value::createFloat(node->valuedouble);
    }
  case cJSON_String:
    return Value::createString(node->valuestring);
  case cJSON_Array: {
    auto arr = Value::createArray();
    size_t index = 0;
    cJSON *elem = node->child;
    while (elem) {
      arr.setElement(index++, parseJSON(elem));
      elem = elem->next;
    }
    return arr;
  }
  case cJSON_Object: {
    auto obj = Value::createObject();
    cJSON *elem = node->child;
    while (elem) {
      obj.setField(elem->string, parseJSON(elem));
      elem = elem->next;
    }
    return obj;
  }
  default:
    return Value::createNull();
  }
}

auto Value::serializeJSON(const Value &value) -> cJSON * {
  switch (value.getType()) {
  case Type::Null:
    return cJSON_CreateNull();
  case Type::Boolean: {
    auto b = value.asBoolean();
    return cJSON_CreateBool(b.value_or(false));
  }
  case Type::Integer: {
    auto i = value.asInteger();
    return cJSON_CreateNumber(i.value_or(0));
  }
  case Type::Float: {
    auto f = value.asFloat();
    return cJSON_CreateNumber(f.value_or(0.0));
  }
  case Type::String: {
    auto s = value.asString();
    return cJSON_CreateString(s.value_or("").c_str());
  }
  case Type::Array: {
    auto arr = cJSON_CreateArray();
    auto elements = value.asArray();
    if (elements) {
      for (const auto &elem : elements.value()) {
        cJSON_AddItemToArray(arr, serializeJSON(elem));
      }
    }
    return arr;
  }
  case Type::Object: {
    auto obj = cJSON_CreateObject();
    auto fields = value.asObject();
    if (fields) {
      for (const auto &[key, val] : fields.value()) {
        cJSON_AddItemToObject(obj, key.c_str(), serializeJSON(val));
      }
    }
    return obj;
  }
  default:
    return cJSON_CreateNull();
  }
}