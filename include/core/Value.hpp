#ifndef _H_CUBE_CORE_VALUE_
#define _H_CUBE_CORE_VALUE_
#include "core/Object.hpp"
#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::core {
class Value : public Object {
public:
  enum class Type { NIL, NUMBER, STRING, BOOLEAN, ARRAY, OBJECT };

private:
  std::any _value;
  Type _type;

public:
  Value() : _type(Type::NIL) {};
  Value(const Value &another) = default;
  auto getType() const -> const Type &;
  auto setNil() -> Value &;
  auto setNumber(double value) -> Value &;
  auto setBoolean(bool value) -> Value &;
  auto setString(const std::string &value) -> Value &;
  auto setArray() -> Value &;
  auto setObject() -> Value &;

  auto isNil() const -> bool;
  auto isNumber() const -> bool;
  auto isString() const -> bool;
  auto isBoolean() const -> bool;
  auto isArray() const -> bool;
  auto isObject() const -> bool;

  auto asNumber() const -> const double *;
  auto asString() const -> const std::string *;
  auto asBoolean() const -> const bool *;
  auto asArray() const -> const std::vector<Value> *;
  auto asObject() const -> const std::unordered_map<std::string, Value> *;

  auto asNumber() -> double *;
  auto asString() -> std::string *;
  auto asBoolean() -> bool *;
  auto asArray() -> std::vector<Value> *;
  auto asObject() -> std::unordered_map<std::string, Value> *;

  auto getNumber(const double &val = {}) const -> const double &;
  auto getString(const std::string &value = {}) const -> const std::string &;
  auto getBoolean(bool val = false) const -> bool;
  auto getArray(const std::vector<Value> &val = {}) const
      -> const std::vector<Value> &;
  auto getObject(const std::unordered_map<std::string, Value> &val = {}) const
      -> const std::unordered_map<std::string, Value> &;

  auto getField(const std::string &key, const Value &val = {}) const
      -> const Value &;
  auto getIndex(size_t idx, const Value &val = {}) const -> const Value &;
  auto getLength() const -> size_t;

  std::string toJSON() const;

public:
  static std::shared_ptr<Value> fromJSON(const std::string &src);
};
} // namespace cube::core
#endif