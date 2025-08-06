#include "core/Object.hpp"
#include <algorithm>
#include <limits>
namespace cube::core {
Object::Object(Object *parent) : _parent(parent) {
  if (_parent) {
    _parent->_children.push_back(this);
    _index = _parent->_children.size() - 1;
  } else {
    _index = std::numeric_limits<size_t>::max();
  }
}

Object::~Object() {
  if (_parent) {
    setParent(nullptr);
  }
  for (const auto &child : _children) {
    child->_parent = NULL;
    child->_index = std::numeric_limits<size_t>::max();
    delete child;
  }
  _children.clear();
}

Object *Object::getParent() { return _parent; }

const Object *Object::getParent() const { return _parent; }

Object *Object::setParent(Object *parent) {
  if (_parent == parent) {
    return this;
  }
  if (_parent) {
    if (_index < _parent->_children.size()) {
      if (_index != _parent->_children.size() - 1) {
        _parent->_children.back()->_index = _index;
        std::swap(_parent->_children[_index], _parent->_children.back());
      }
      _parent->_children.pop_back();
    }
    _index = std::numeric_limits<size_t>::max();
  }
  _parent = parent;
  if (_parent) {
    _parent->_children.push_back(this);
    _index = _parent->_children.size() - 1;
  }
  return this;
}

} // namespace cube::core