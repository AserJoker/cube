#pragma once
#include "Type.hpp"
#include "core/Object.hpp"
#include <any>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
namespace cube::reflection {
class Variable : public core::Object {
public:
  template <class T> using Getter = std::function<T()>;
  template <class T> using Setter = std::function<void(const T &)>;

  using GetField = std::function<Variable *(const std::string &)>;
  using SetField = std::function<Variable *(const std::string &, Variable *)>;
  using DelField = std::function<bool(const std::string &)>;
  using HasField = std::function<bool(const std::string &)>;
  using GetIndex = std::function<Variable *(size_t)>;
  using SetIndex = std::function<Variable *(size_t, Variable *)>;
  using GetSize = std::function<size_t()>;
  using SetSize = std::function<void(size_t)>;
  using GetKeys = std::function<std::vector<std::string>()>;
  struct DictProxy {
    GetField getField;
    SetField setField;
    DelField delField;
    HasField hasField;
    GetKeys getKeys;
  };
  struct ArrayProxy {
    GetIndex getIndex;
    SetIndex setIndex;
    GetSize getSize;
    SetSize setSize;
  };

private:
  enum class Kind { VALUE, REF, ACCESSOR };

private:
  std::any _value;
  Type *_type;
  Kind _kind;

public:
  Variable();
  Type *getType();
  Variable *setNull();
  bool isNull();
  Variable *reset();
  Variable *setInteger(int64_t value = 0);
  Variable *setInteger(int64_t *value);
  Variable *setInteger(const Getter<int64_t> &getter,
                       const Setter<int64_t> &setter);
  int64_t getInteger();
  Variable *setNumber(double value = 0);
  Variable *setNumber(double *value);
  Variable *setNumber(const Getter<double> &getter,
                      const Setter<double> &setter);
  double getNumber();
  Variable *setBoolean(bool value = false);
  Variable *setBoolean(bool *value);
  Variable *setBoolean(const Getter<bool> &getter, const Setter<bool> &setter);
  bool getBoolean();
  Variable *setString(const std::string &value = "");
  Variable *setString(std::string *value);
  Variable *setString(const Getter<std::string> &getter,
                      const Setter<std::string> &setter);
  std::string getString();
  Variable *setArray();
  Variable *setDict();
  Variable *setArray(const ArrayProxy &proxy);
  Variable *setDict(const DictProxy &proxy);
  Variable *getIndex(size_t idx);
  Variable *setIndex(size_t idx, Variable *item);
  size_t getSize();
  void setSize(size_t size);
  Variable *getField(const std::string &name);
  Variable *setField(const std::string &name, Variable *item);
  bool delField(const std::string &name);
  std::vector<std::string> getKeys();
  Variable *setObject(Type *type, core::Object *self);
  core::Object *getObject();
};
} // namespace cube::reflection