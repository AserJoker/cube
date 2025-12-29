#include "core/Buffer.hpp"
#include <cstdint>
#include <cstring>
using namespace cube;
using namespace cube::core;
Buffer::Buffer(size_t size, const void *data) : _size(size) {
  if (size > 0) {
    _data = ::operator new(size);
    if (data) {
      std::memcpy(_data, data, size);
    }
  }
}
Buffer::~Buffer() {
  if (_data) {
    ::operator delete(_data);
    _data = nullptr;
    _size = 0;
  }
}

auto Buffer::getSize() const -> size_t { return _size; }

auto Buffer::getData() const -> const void * { return _data; }

auto Buffer::resize(size_t newSize) -> void {
  if (newSize == _size) {
    return;
  }
  void *newData = nullptr;
  if (newSize > 0) {
    newData = ::operator new(newSize);
    std::memset(newData, 0, newSize);
    if (_data) {
      size_t copySize = (newSize < _size) ? newSize : _size;
      std::memcpy(newData, _data, copySize);
    }
  }
  if (_data) {
    ::operator delete(_data);
  }
  _data = newData;
  _size = newSize;
}

auto Buffer::write(const void *data, size_t size, size_t offset) -> size_t {
  if (!_data || !data) {
    return 0;
  }
  size = _size - offset < size ? _size - offset : size;
  std::memcpy(static_cast<uint8_t *>(_data) + offset, data, size);
  return size;
}

auto Buffer::read(void *data, size_t size, size_t offset) const -> size_t {
  if (!_data || !data) {
    return 0;
  }
  size = _size - offset < size ? _size - offset : size;
  std::memcpy(data, static_cast<uint8_t *>(_data) + offset, size);
  return size;
}