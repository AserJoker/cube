#include "runtime/OpenGLWindow.hpp"
#include "render/OpenGLRenderer.hpp"
#include "runtime/Window.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_video.h>
#include <glad/glad.h>
#include <sstream>
#include <stdexcept>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char *message, const void *userParam) {
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;
  if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
    return;
  }
  std::stringstream ss;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    ss << "Source: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    ss << "Source: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    ss << "Source: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    ss << "Source: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    ss << "Source: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    ss << "Source: Other";
    break;
  }
  ss << ", ";

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    ss << "Type: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    ss << "Type: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    ss << "Type: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    ss << "Type: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    ss << "Type: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    ss << "Type: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    ss << "Type: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    ss << "Type: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    ss << "Type: Other";
    break;
  }
  ss << ", ";
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    ss << "Severity: high";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    ss << "Severity: medium";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    ss << "Severity: low";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    ss << "Severity: notification";
    break;
  }
  throw std::runtime_error(ss.str());
}

namespace cube::runtime {
OpenGLWindow::OpenGLWindow() : Window(createWindow()) {}
void OpenGLWindow::initOpenGLContext(int32_t major, int32_t minor) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, minor);
#if defined(__APPLE__) && defined(__MACH__)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE |
                          SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
  _glContext = SDL_GL_CreateContext(getHandle());
  if (!_glContext) {
    throw std::runtime_error(SDL_GetError());
  }
  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glDebugOutput, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                        GL_TRUE);
  _renderer = create<render::OpenGLRenderer>();
  _renderer->setViewport(0, 0, CUBE_DEFAULT_WINDOW_WIDTH,
                         CUBE_DEFAULT_WINDOW_HEIGHT);
}
OpenGLWindow::~OpenGLWindow() {
  if (_glContext) {
    SDL_GL_DestroyContext(_glContext);
    _glContext = nullptr;
  }
}
SDL_Window *OpenGLWindow::createWindow() {
  return SDL_CreateWindow("cube", CUBE_DEFAULT_WINDOW_WIDTH,
                          CUBE_DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
}
void OpenGLWindow::present() { SDL_GL_SwapWindow(getHandle()); }

render::IRenderer *OpenGLWindow::getRenderer() { return _renderer; }
} // namespace cube::runtime