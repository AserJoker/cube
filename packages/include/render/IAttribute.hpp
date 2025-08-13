#pragma once
#include "IBuffer.hpp"
#include "render/IBuffer.hpp"
#include <cstdint>
namespace cube::render {
class IAttribute : public IBuffer {
public:
  virtual uint32_t getStride() const = 0;
  virtual bool isNormalize() const = 0;
};
}; // namespace cube::render