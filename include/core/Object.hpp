#ifndef _H_CUBE_CORE_OBJECT_
#define _H_CUBE_CORE_OBJECT_
namespace cube::core {
class Object {
protected:
  Object() = default;

public:
  virtual ~Object() = default;
};
} // namespace cube::core
#endif