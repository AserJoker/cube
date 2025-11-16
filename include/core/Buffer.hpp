#pragma once
#include "core/Object.hpp"
namespace cube::core {
class Buffer : public Object {
private:
  size_t _size = 0;
  void *_data = nullptr;

public:
  Buffer(size_t size = 0, const void *data = nullptr);
  ~Buffer() override;
  auto getSize() const -> size_t;
  auto getData() const -> const void *;
  auto resize(size_t newSize) -> void;
  auto write(const void *data, size_t size, size_t offset = 0) -> size_t;
  auto read(void *data, size_t size, size_t offset = 0) const -> size_t;
};
} // namespace cube::core