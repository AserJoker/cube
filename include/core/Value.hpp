#pragma once
#include "core/Object.hpp"
#include <cjson/cJSON.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::core {
class Value : public Object {
public:
  enum class Type { Null, Boolean, Integer, Float, String, Array, Object };

private:
  struct Node : public Object {
    Type type = Type::Null;
  };
  struct NullNode : public Node {
    NullNode() { type = Type::Null; }
  };
  struct BooleanNode : public Node {
    bool value = false;
    BooleanNode(bool v) : value(v) { type = Type::Boolean; }
  };
  struct IntegerNode : public Node {
    int64_t value = 0;
    IntegerNode(int64_t v) : value(v) { type = Type::Integer; }
  };
  struct FloatNode : public Node {
    double value = 0.0;
    FloatNode(double v) : value(v) { type = Type::Float; }
  };
  struct StringNode : public Node {
    std::string value;
    StringNode(const std::string &v) : value(v) { type = Type::String; }
  };
  struct ArrayNode : public Node {
    std::vector<std::shared_ptr<Node>> values;
    ArrayNode() { type = Type::Array; }
  };
  struct ObjectNode : public Node {
    std::unordered_map<std::string, std::shared_ptr<Node>> values;
    ObjectNode() { type = Type::Object; }
  };
  std::shared_ptr<Node> _node;

public:
  Value();
  ~Value() override;
  static auto createNull() -> Value;
  static auto createBoolean(bool v) -> Value;
  static auto createInteger(int64_t v) -> Value;
  static auto createFloat(double v) -> Value;
  static auto createString(const std::string &v) -> Value;
  static auto createArray() -> Value;
  static auto createObject() -> Value;
  auto asBoolean() const -> std::optional<bool>;
  auto asInteger() const -> std::optional<int64_t>;
  auto asFloat() const -> std::optional<double>;
  auto asString() const -> std::optional<std::string>;
  auto asArray() const -> std::optional<std::vector<Value>>;
  auto asObject() const
      -> std::optional<std::unordered_map<std::string, Value>>;
  auto getType() const -> Type;
  auto getLength() const -> std::optional<size_t>;
  auto getKeys() const -> std::optional<std::vector<std::string>>;
  auto get(const std::string &path) const -> std::optional<Value>;
  auto getField(const std::string &key) const -> std::optional<Value>;
  auto getElement(size_t index) const -> std::optional<Value>;
  auto setNull() -> void;
  auto setBoolean(bool v) -> void;
  auto setInteger(int64_t v) -> void;
  auto setFloat(double v) -> void;
  auto setString(const std::string &v) -> void;
  auto setArray() -> void;
  auto setObject() -> void;
  auto set(const std::string &path, const Value &value) -> bool;
  auto setField(const std::string &key, const Value &value) -> bool;
  auto setElement(size_t index, const Value &value) -> bool;
  auto setLength(size_t length) -> bool;
  auto removeField(const std::string &key) -> bool;
  auto appendElement(const Value &value) -> bool;

public:
  static auto parseJSON(cJSON *node) -> Value;
  static auto serializeJSON(const Value &value) -> cJSON *;
};

} // namespace cube::core