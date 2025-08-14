#pragma once
#include "core/Event.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
namespace cube::runtime {
class EventSDL : public core::Event<"cube.runtime.SDL"> {
private:
  SDL_Event _event;

public:
  EventSDL(const SDL_Event &event) : _event(event) {};
  const SDL_Event &getEvent() const { return _event; }
};
} // namespace cube::runtime