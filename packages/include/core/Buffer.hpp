#pragma once
#include "core/Object.hpp"
#include <cstdint>
namespace cube::core {
class Buffer : public Object {
private:
  uint8_t *_data;
  size_t _length;

public:
  Buffer(size_t length = 0, void *data = nullptr);
  ~Buffer() override;
  size_t getLength() const;
  const void *getData() const;
  void *getData();
  void resize(size_t length);
  void *read(size_t offset = 0);
  void write(size_t offset, size_t length, void *data);
};
}; // namespace cube::core