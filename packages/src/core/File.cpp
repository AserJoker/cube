#include "core/File.hpp"
#include "core/Buffer.hpp"
#include <stdexcept>
#include <stdio.h>
namespace cube::core {
File::File(const std::string &path) : _handle(nullptr) {
  if (!path.empty()) {
    open(path);
  }
}
File::~File() { close(); }

void File::close() {
  if (_handle) {
    fclose(_handle);
    _handle = nullptr;
  }
}

void File::open(const std::string &path) {
  _handle = fopen(path.c_str(), "rb+");
  if (!_handle) {
    throw std::runtime_error("cannot open file: " + path);
  }
}
size_t File::tell() { return _ftelli64(_handle); }
void File::seek(size_t offset, int pos) { _fseeki64(_handle, offset, pos); }
core::Buffer *File::read(size_t size) {
  if (size == 0) {
    seek(0, SEEK_END);
    size = tell();
    seek(0, SEEK_SET);
  }
  core::Buffer *buffer = create<Buffer>(size);
  fread(buffer->getData(), size, 1, _handle);
  return buffer;
}
void File::write(core::Buffer *data, size_t offset, size_t length) {
  fwrite((uint8_t *)data->getData() + offset, length, 1, _handle);
}
}; // namespace cube::core