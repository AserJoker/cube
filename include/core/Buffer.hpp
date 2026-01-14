#ifndef _H_CUBE_CORE_BUFFER_
#define _H_CUBE_CORE_BUFFER_
#include "core/Object.hpp"
#include <cstddef>
namespace cube::core {
class Buffer : public Object {
private:
  void *_data = nullptr;
  size_t _size = 0;

public:
  Buffer(size_t size = 0, const void *data = nullptr);
  Buffer(const Buffer &another);
  Buffer(Buffer &&another);
  ~Buffer() override;
  void *getData();
  const void *getData() const;
  size_t getSize() const;
  void setData(size_t size, const void *data);
  void resize(size_t size);
  auto read(void *dst, size_t offset = 0, size_t len = (size_t)-1)
      -> void const;
  auto write(const void *src, size_t offset = 0, size_t len = (size_t)-1)
      -> void;
};
} // namespace cube::core
#endif