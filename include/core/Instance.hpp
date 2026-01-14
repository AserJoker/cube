#ifndef _H_CUBE_CORE_INSTANCE_
#define _H_CUBE_CORE_INSTANCE_
#include "Object.hpp"
namespace cube::core {
class Instance : public Object {
public:
  Instance() = default;
  Instance(const Instance &) = delete;
};
} // namespace cube::core
#endif