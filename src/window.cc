#include "window.h"

Window::Window(const char *title, int width, int height) {
  // Create SDL (Simple Directmedia Layer) window.
  sdl_window_ptr_ = SDL_CreateWindow(
      title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  window_id_ = SDL_GetWindowID(sdl_window_ptr_);

  // Create OpenGLES 2 context on SDL window.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetSwapInterval(1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GLContext glc = SDL_GL_CreateContext(sdl_window_ptr_);
}