//
// Window and input event handling
//

#pragma once

#include <SDL2/SDL_opengles2.h>

#include "camera.h"
#include "window.h"

class EventHandler {
public:
  EventHandler(Window &window, Camera &camera);

  void processEvents();
  Camera &camera() { return mCamera; }
  void swapWindow();

private:
  // Camera.
  Camera &mCamera;

  // Window.
  SDL_Window *mpWindow;
  Uint32 mWindowID;
  void InitializeViewport(int width, int height);
  void initWindow(const char *title);

  // Mouse input
  const float cMouseWheelZoomDelta;
  bool mMouseButtonDown;
  int mMouseButtonDownX;
  int mMouseButtonDownY;
  int mMousePositionX;
  int mMousePositionY;

  // Finger input.
  bool mFingerDown;
  float mFingerDownX;
  float mFingerDownY;
  long long mFingerDownId;

  // Pinch input.
  const float cPinchZoomThreshold;
  const float cPinchScale;
  bool mPinch;

  // Events.
  void zoomEventMouse(bool mouseWheelDown, int x, int y);
  void zoomEventPinch(float pinchDist, float pinchX, float pinchY);
  void panEventMouse(int x, int y);
  void panEventFinger(float x, float y);
};
