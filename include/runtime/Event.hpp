#ifndef _H_CUBE_RUNTIME_SIMPLE_EVENT_
#define _H_CUBE_RUNTIME_SIMPLE_EVENT_
#include "core/Object.hpp"
#include "core/TString.hpp"
namespace cube::runtime {
template <core::TString t> class Event : public core::Object {
public:
  static inline constexpr const char *type = t.value;
};

using PreInitializeEvent = Event<"cube.runtime.PreInitialize">;
using InitializeEvent = Event<"cube.runtime.InitializeEvent">;
using PostInitializeEvent = Event<"cube.runtime.PostInitializeEvent">;
using QuitEvent = Event<"cube.runtime.QuitEvent">;
using TickEvent = Event<"cube.runtime.TickEvent">;
using RandomTickEvent = Event<"cube.runtime.RandomTickEvent">;
} // namespace cube::runtime
#endif