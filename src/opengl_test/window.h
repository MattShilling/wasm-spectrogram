#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

/**
 * @brief Eventually, we should make a base class so the event handler
 * doesn't need to know if this is a SDL window or another abstracted
 * window.
 */
class Window {
public:
  Window(const char *title, int width, int height);

  SDL_Window *GetWindow() { return sdl_window_ptr_; }
  Uint32 GetWindowId() { return window_id_; }

private:
  SDL_Window *sdl_window_ptr_;
  Uint32 window_id_;
};
