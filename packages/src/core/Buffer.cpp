#include "core/Buffer.hpp"
#include <cstring>
#include <new>
namespace cube::core {
Buffer::Buffer(size_t length, void *data) : _length(length), _data(nullptr) {
  if (_length) {
    _data = (uint8_t *)::operator new(length + 1);
    std::memset(_data, 0, _length + 1);
  }
  if (data) {
    std::memcpy(_data, data, _length);
  }
}
Buffer::~Buffer() {
  if (_data) {
    ::operator delete(_data);
    _data = nullptr;
  }
}
size_t Buffer::getLength() const { return _length; }

const void *Buffer::getData() const { return _data; }

void *Buffer::getData() { return _data; }

void Buffer::resize(size_t length) {
  if (length != _length) {
    uint8_t *data = (uint8_t *)::operator new(length + 1);
    std::memset(data, 0, length + 1);
    std::memcpy(data, _data, length < _length ? length : _length);
    ::operator delete(_data);
    _data = data;
    _length = length;
  }
}

void *Buffer::read(size_t offset) { return &_data[offset]; }

void Buffer::write(size_t offset, size_t length, void *data) {
  if (offset + length > _length) {
    resize(offset + length);
  }
  std::memcpy(&_data[offset], data, length);
}
}; // namespace cube::core