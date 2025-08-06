#pragma once
#include "core/Object.hpp"
namespace cube::reflection {
class Type : public core::Object {
public:
  enum class Kind {
    NIL,
    NUMBER,
    INTEGER,
    STRING,
    BOOLEAN,
    DICT,
    ARRAY,
    OBJECT
  };

private:
  Kind _kind;

public:
  Type(const Kind &kind);
  const Kind &getKind() const;
  virtual const char *getTypeName() const = 0;
};
} // namespace cube::reflection