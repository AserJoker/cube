#include "reflection/Variable.hpp"
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include "reflection/ArrayType.hpp"
#include "reflection/BooleanType.hpp"
#include "reflection/DictType.hpp"
#include "reflection/IntegerType.hpp"
#include "reflection/NullType.hpp"
#include "reflection/NumberType.hpp"
#include "reflection/StringType.hpp"
#include "reflection/Type.hpp"
#include <any>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

namespace cube::reflection {

template <class T> struct Accessor {
  Variable::Getter<T> getter;
  Variable::Setter<T> setter;
};

Variable::Variable() : _kind(Kind::VALUE), _type(nullptr) {
  _type = core::Singleton<NullType>::get();
}

Type *Variable::getType() { return _type; }

Variable *Variable::setNull() {
  _value.reset();
  _type = core::Singleton<NullType>::get();
  _kind = Kind::VALUE;
  return this;
}
bool Variable::isNull() { return _type->getKind() == Type::Kind::NIL; }

Variable *Variable::reset() {
  _value.reset();
  _kind = Kind::VALUE;
  _type = core::Singleton<NullType>::get();
  return this;
}

Variable *Variable::setInteger(int64_t value) {
  if (_type->getKind() == Type::Kind::INTEGER) {
    if (_kind == Kind::VALUE) {
      _value = value;
    } else if (_kind == Kind::REF) {
      *std::any_cast<int64_t *>(_value) = value;
    } else if (_kind == Kind::ACCESSOR) {
      auto &accessor = std::any_cast<Accessor<int64_t> &>(_value);
      if (accessor.setter) {
        accessor.setter(value);
      } else {
        throw std::runtime_error("Failed to set integer, variable is readonly");
      }
    }
  } else {
    _value = value;
    _type = core::Singleton<IntegerType>::get();
    _kind = Kind::VALUE;
  }
  return this;
}
Variable *Variable::setInteger(int64_t *value) {
  _value = value;
  _type = core::Singleton<IntegerType>::get();
  _kind = Kind::REF;
  return this;
}
Variable *Variable::setInteger(const Getter<int64_t> &getter,
                               const Setter<int64_t> &setter) {
  if (!getter) {
    throw std::runtime_error(
        "Failed to set integer accessor, getter is required");
  }
  _value = Accessor{getter, setter};
  _type = core::Singleton<IntegerType>::get();
  _kind = Kind::ACCESSOR;
  return this;
}
int64_t Variable::getInteger() {
  if (_kind == Kind::REF) {
    return *std::any_cast<int64_t *>(_value);
  } else if (_kind == Kind::ACCESSOR) {
    return std::any_cast<Accessor<int64_t>>(_value).getter();
  }
  return std::any_cast<int64_t>(_value);
}
Variable *Variable::setNumber(double value) {
  if (_type->getKind() == Type::Kind::NUMBER) {
    if (_kind == Kind::VALUE) {
      _value = value;
    } else if (_kind == Kind::REF) {
      *std::any_cast<double *>(_value) = value;
    } else if (_kind == Kind::ACCESSOR) {
      auto &accessor = std::any_cast<Accessor<double> &>(_value);
      if (accessor.setter) {
        accessor.setter(value);
      } else {
        throw std::runtime_error("Failed to set number, variable is readonly");
      }
    }
  } else {
    _value = value;
    _type = core::Singleton<NumberType>::get();
    _kind = Kind::VALUE;
  }
  return this;
}
Variable *Variable::setNumber(double *value) {
  _value = value;
  _type = core::Singleton<NumberType>::get();
  _kind = Kind::REF;
  return this;
}
Variable *Variable::setNumber(const Getter<double> &getter,
                              const Setter<double> &setter) {
  if (!getter) {
    throw std::runtime_error(
        "Failed to set number accessor, getter is required");
  }
  _value = Accessor{getter, setter};
  _type = core::Singleton<NumberType>::get();
  _kind = Kind::ACCESSOR;
  return this;
}
double Variable::getNumber() {
  if (_kind == Kind::REF) {
    return *std::any_cast<double *>(_value);
  } else if (_kind == Kind::ACCESSOR) {
    return std::any_cast<Accessor<double>>(_value).getter();
  }
  return std::any_cast<double>(_value);
}
Variable *Variable::setBoolean(bool value) {
  if (_type->getKind() == Type::Kind::BOOLEAN) {
    if (_kind == Kind::VALUE) {
      _value = value;
    } else if (_kind == Kind::REF) {
      *std::any_cast<bool *>(_value) = value;
    } else if (_kind == Kind::ACCESSOR) {
      auto &accessor = std::any_cast<Accessor<bool> &>(_value);
      if (accessor.setter) {
        accessor.setter(value);
      } else {
        throw std::runtime_error("Failed to set boolean, variable is readonly");
      }
    }
  } else {
    _value = value;
    _type = core::Singleton<BooleanType>::get();
    _kind = Kind::VALUE;
  }
  return this;
}
Variable *Variable::setBoolean(bool *value) {
  _value = value;
  _type = core::Singleton<BooleanType>::get();
  _kind = Kind::REF;
  return this;
}
Variable *Variable::setBoolean(const Getter<bool> &getter,
                               const Setter<bool> &setter) {
  if (!getter) {
    throw std::runtime_error(
        "Failed to set boolean accessor, getter is required");
  }
  _value = Accessor{getter, setter};
  _type = core::Singleton<BooleanType>::get();
  _kind = Kind::ACCESSOR;
  return this;
}
bool Variable::getBoolean() {
  if (_kind == Kind::REF) {
    return *std::any_cast<bool *>(_value);
  } else if (_kind == Kind::ACCESSOR) {
    return std::any_cast<Accessor<bool>>(_value).getter();
  }
  return std::any_cast<bool>(_value);
}
Variable *Variable::setString(const std::string &value) {
  if (_type->getKind() == Type::Kind::BOOLEAN) {
    if (_kind == Kind::VALUE) {
      _value = value;
    } else if (_kind == Kind::REF) {
      *std::any_cast<std::string *>(_value) = value;
    } else if (_kind == Kind::ACCESSOR) {
      auto &accessor = std::any_cast<Accessor<std::string> &>(_value);
      if (accessor.setter) {
        accessor.setter(value);
      } else {
        throw std::runtime_error("Failed to set string, variable is readonly");
      }
    }
  } else {
    _value = value;
    _type = core::Singleton<StringType>::get();
    _kind = Kind::VALUE;
  }
  return this;
}
Variable *Variable::setString(std::string *value) {
  _value = value;
  _type = core::Singleton<StringType>::get();
  _kind = Kind::REF;
  return this;
}
Variable *Variable::setString(const Getter<std::string> &getter,
                              const Setter<std::string> &setter) {
  if (!getter) {
    throw std::runtime_error(
        "Failed to set string accessor, getter is required");
  }
  _value = Accessor{getter, setter};
  _type = core::Singleton<BooleanType>::get();
  _kind = Kind::ACCESSOR;
  return this;
}
std::string Variable::getString() {
  if (_kind == Kind::REF) {
    return *std::any_cast<std::string *>(_value);
  } else if (_kind == Kind::ACCESSOR) {
    return std::any_cast<Accessor<std::string>>(_value).getter();
  }
  return std::any_cast<std::string>(_value);
}
Variable *Variable::setArray() {
  _type = core::Singleton<ArrayType>::get();
  _kind = Kind::VALUE;
  _value = std::vector<Variable *>{};
  return this;
}
Variable *Variable::setDict() {
  _type = core::Singleton<DictType>::get();
  _kind = Kind::VALUE;
  _value = std::unordered_map<std::string, Variable *>{};
  return this;
}
Variable *Variable::setArray(const ArrayProxy &proxy) {
  if (!proxy.getIndex || !proxy.getSize) {
    throw std::runtime_error(
        "Failed to set array proxy, getIndex & getSize is required");
  }
  _type = core::Singleton<ArrayType>::get();
  _kind = Kind::ACCESSOR;
  _value = proxy;
  return this;
}
Variable *Variable::setDict(const DictProxy &proxy) {
  if (!proxy.getField || !proxy.hasField) {
    throw std::runtime_error(
        "Failed to set dict proxy, getField & hasField is required");
  }
  _type = core::Singleton<DictType>::get();
  _kind = Kind::ACCESSOR;
  _value = proxy;
  return this;
}
Variable *Variable::getIndex(size_t idx) {
  if (_type->getKind() == Type::Kind::ARRAY) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<ArrayProxy &>(_value);
      if (idx >= proxy.getSize()) {
        if (proxy.setSize) {
          proxy.setSize(idx + 1);
        } else {
          return nullptr;
        }
      }
      if (proxy.getIndex(idx) == nullptr) {
        if (proxy.setIndex) {
          proxy.setIndex(idx, create<Variable>());
        }
      }
      return proxy.getIndex(idx);
    }
    auto &array = std::any_cast<std::vector<Variable *> &>(_value);
    if (idx >= array.size()) {
      array.resize(idx + 1, nullptr);
    }
    if (!array[idx]) {
      array[idx] = create<Variable>();
    }
    return array[idx];
  }
  return nullptr;
}
Variable *Variable::setIndex(size_t idx, Variable *item) {
  if (_type->getKind() == Type::Kind::ARRAY) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<ArrayProxy &>(_value);
      if (idx >= proxy.getSize()) {
        if (proxy.setSize) {
          proxy.setSize(idx + 1);
        } else {
          throw std::runtime_error("Failed to set index, setSize is undefined");
        }
      }
      if (!proxy.setIndex) {
        throw std::runtime_error("Failed to set index, setIndex is undefined");
      }
      return proxy.setIndex(idx, item);
    }
    auto &array = std::any_cast<std::vector<Variable *> &>(_value);
    if (idx >= array.size()) {
      array.resize(idx + 1, nullptr);
    }
    if (array[idx]) {
      delete array[idx];
      array[idx] = item;
      item->setParent(this);
    }
    return this;
  }
  return nullptr;
}
size_t Variable::getSize() {
  if (_type->getKind() == Type::Kind::ARRAY) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<ArrayProxy &>(_value);
      return proxy.getSize();
    }
    auto &array = std::any_cast<std::vector<Variable *> &>(_value);
    return array.size();
  }
  return 0;
}
void Variable::setSize(size_t size) {
  if (_type->getKind() == Type::Kind::ARRAY) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<ArrayProxy &>(_value);
      if (!proxy.setSize) {
        throw std::runtime_error(
            "Failed to set array size, setSize is undefined");
      }
      proxy.setSize(size);
    }
    auto &array = std::any_cast<std::vector<Variable *> &>(_value);
    array.resize(size, nullptr);
  }
}
Variable *Variable::getField(const std::string &name) {
  if (_type->getKind() == Type::Kind::DICT) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<DictProxy &>(_value);
      if (!proxy.hasField(name)) {
        if (proxy.setField) {
          proxy.setField(name, create<Variable>());
        } else {
          return nullptr;
        }
      }
      return proxy.getField(name);
    }
    auto &dict =
        std::any_cast<std::unordered_map<std::string, Variable *> &>(_value);
    if (!dict.contains(name)) {
      dict[name] = create<Variable>();
    }
    return dict[name];
  }
  return nullptr;
}
Variable *Variable::setField(const std::string &name, Variable *item) {
  if (_type->getKind() == Type::Kind::DICT) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<DictProxy &>(_value);
      if (!proxy.setField) {
        throw std::runtime_error("Failed to set field, setField is undefined");
      }
      return proxy.setField(name, item);
    }
    auto &dict =
        std::any_cast<std::unordered_map<std::string, Variable *> &>(_value);
    if (dict.contains(name)) {
      delete dict[name];
    }
    dict[name] = item;
    item->setParent(this);
    return this;
  }
  return nullptr;
}
bool Variable::delField(const std::string &name) {
  if (_type->getKind() == Type::Kind::DICT) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<DictProxy &>(_value);
      if (!proxy.delField) {
        throw std::runtime_error(
            "Failed to delete field, delField is undefined");
      }
      return proxy.delField(name);
    }
    auto &dict =
        std::any_cast<std::unordered_map<std::string, Variable *> &>(_value);
    if (dict.contains(name)) {
      if (dict[name]) {
        delete dict[name];
      }
      dict.erase(name);
      return true;
    }
  }
  return false;
}
bool Variable::hasField(const std::string &name) {
  if (_type->getKind() == Type::Kind::DICT) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<DictProxy &>(_value);
      if (!proxy.hasField) {
        throw std::runtime_error(
            "Failed to check field, hasField is undefined");
      }
      return proxy.hasField(name);
    }
    auto &dict =
        std::any_cast<std::unordered_map<std::string, Variable *> &>(_value);
    if (dict.contains(name)) {
      return true;
    }
  }
  return false;
}
std::vector<std::string> Variable::getKeys() {
  std::vector<std::string> keys;
  if (_type->getKind() == Type::Kind::DICT) {
    if (_kind == Kind::ACCESSOR) {
      auto &proxy = std::any_cast<DictProxy &>(_value);
      if (!proxy.delField) {
        throw std::runtime_error("Failed to get keys, getKeys is undefined");
      }
      return proxy.getKeys();
    }
    auto &dict =
        std::any_cast<std::unordered_map<std::string, Variable *> &>(_value);
    for (auto &[key, _] : dict) {
      keys.push_back(key);
    }
  }
  return keys;
}
Variable *Variable::setObject(Type *type, core::Object *self) {
  _value = self;
  _type = type;
  return this;
}
core::Object *Variable::getObject() {
  return std::any_cast<core::Object *>(_value);
}
} // namespace cube::reflection