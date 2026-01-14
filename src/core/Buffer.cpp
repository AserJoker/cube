#include "core/Buffer.hpp"
#include <cstdint>
#include <cstring>
#include <new>
using namespace cube;
using namespace cube::core;

Buffer::Buffer(size_t size, const void *data) : _size(size) {
  if (_size) {
    _data = ::operator new(_size);
    if (data) {
      memcpy(_data, data, _size);
    } else {
      memset(_data, 0, _size);
    }
  }
}
Buffer::Buffer(const Buffer &another) {
  resize(another._size);
  write(another._data);
}
Buffer::Buffer(Buffer &&another) {
  _size = another._size;
  _data = another._data;
  another._size = 0;
  another._data = nullptr;
}
Buffer::~Buffer() {
  if (_data) {
    ::operator delete(_data);
  }
  _data = nullptr;
  _size = 0;
}
void *Buffer::getData() { return _data; }
const void *Buffer::getData() const { return _data; }
size_t Buffer::getSize() const { return _size; }
void Buffer::setData(size_t size, const void *data) {
  if (_data) {
    ::operator delete(_data);
  }
  _size = size;
  if (_size) {
    _data = ::operator new(_size);
    memcpy(_data, data, _size);
  }
}
void Buffer::resize(size_t size) {
  if (size != _size) {
    void *data = nullptr;
    if (size) {
      data = ::operator new(size);
      size_t sz = size < _size ? size : _size;
      memcpy(data, _data, sz);
      if (sz < size) {
        memset(data, 0, size - sz);
      }
    }
    _size = size;
    ::operator delete(_data);
    _data = data;
  }
}
auto Buffer::read(void *dst, size_t offset, size_t len) -> void const {
  if (len == (size_t)-1) {
    len = _size;
  }
  if (offset >= _size) {
    return;
  }
  if (offset + len > _size) {
    len = _size - offset;
  }
  memcpy(dst, (uint8_t *)_data + offset, len);
};
auto Buffer::write(const void *src, size_t offset, size_t len) -> void {
  if (len == (size_t)-1) {
    len = _size;
  }
  if (offset >= _size) {
    return;
  }
  if (offset + len > _size) {
    len = _size - offset;
  }
  memcpy((uint8_t *)_data + offset, src, len);
};