#pragma once
#include "core/Buffer.hpp"
#include "core/Object.hpp"
#include <cstddef>
#include <cstdio>
#include <string>
namespace cube::core {
class File : public core::Object {
private:
  FILE *_handle;

public:
  File(const std::string &path = "");
  ~File() override;
  void close();
  void open(const std::string &path = "");
  size_t tell();
  void seek(size_t offset, int pos);
  core::Buffer *read(size_t size = 0);
  void write(core::Buffer *data, size_t offset = 0, size_t length = 0);
};
} // namespace cube::core