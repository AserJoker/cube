#pragma once
#include <typeinfo>
#include <utility>
#include <vector>

namespace cube::core {
class Object {
private:
  Object *_parent;
  std::vector<Object *> _children;
  size_t _index;

public:
  Object(Object *parent = nullptr);
  virtual ~Object();
  Object *getParent();
  const Object *getParent() const;
  Object *setParent(Object *parent);
  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;
  template <class K> constexpr K *cast() { return dynamic_cast<K *>(this); }
  template <class K> K *assertAndCast() {
    K *ins = cast<K>();
    if (!ins) {
      throw std::bad_cast();
    }
    return ins;
  }
  template <class T> T *create(auto &&...args) {
    T *object = createObject<T>(std::forward<decltype(args)>(args)...);
    object->setParent(this);
    return object;
  }

public:
  template <class T> static T *createObject(auto &&...args) {
    T *object = new T(std::forward<decltype(args)>(args)...);
    return object;
  }
};
} // namespace cube::core