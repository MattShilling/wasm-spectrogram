//
// Window and input event handling
//
#include "events.h"

#include <SDL2/SDL_opengles2.h>
#include <algorithm>

#include "camera.h"
#include "window.h"

// #define EVENTS_DEBUG

EventHandler::EventHandler(Window &window, Camera &camera)
    : mCamera(camera), mpWindow(window.GetWindow()),
      mWindowID(window.GetWindowId()),

      // Mouse input.
      cMouseWheelZoomDelta(0.05f), mMouseButtonDown(false),
      mMouseButtonDownX(0), mMouseButtonDownY(0), mMousePositionX(0),
      mMousePositionY(0),

      // Finger input
      mFingerDown(false), mFingerDownX(0.0f), mFingerDownY(0.0f),
      mFingerDownId(0),

      // Pinch input
      cPinchZoomThreshold(0.001f), cPinchScale(8.0f), mPinch(false) {
  // Set clear color to black.
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Initialize viewport.
  SetViewport(camera.windowSize().width, camera.windowSize().height);
}

void EventHandler::SetViewport(int width, int height) {
  glViewport(0, 0, width, height);
  mCamera.setWindowSize(width, height);
}

void EventHandler::swapWindow() { SDL_GL_SwapWindow(mpWindow); }

void EventHandler::zoomEventMouse(bool mouseWheelDown, int x, int y) {
  float preZoomWorldX, preZoomWorldY;
  mCamera.windowToWorldCoords(mMousePositionX, mMousePositionY, preZoomWorldX,
                              preZoomWorldY);

  // Zoom by scaling up/down in 0.05 increments.
  float zoomDelta =
      mouseWheelDown ? -cMouseWheelZoomDelta : cMouseWheelZoomDelta;
  mCamera.setZoomDelta(zoomDelta);

  // Zoom to point: Keep the world coords under mouse position the same before
  // and after the zoom
  float postZoomWorldX, postZoomWorldY;
  mCamera.windowToWorldCoords(mMousePositionX, mMousePositionY, postZoomWorldX,
                              postZoomWorldY);
  Vec2 deltaWorld = {postZoomWorldX - preZoomWorldX,
                     postZoomWorldY - preZoomWorldY};
  mCamera.setPanDelta(deltaWorld);
}

void EventHandler::zoomEventPinch(float pinchDist, float pinchX, float pinchY) {
  float preZoomWorldX, preZoomWorldY;
  mCamera.normWindowToWorldCoords(pinchX, pinchY, preZoomWorldX, preZoomWorldY);

  // Zoom in/out by positive/negative mPinch distance
  float zoomDelta = pinchDist * cPinchScale;
  mCamera.setZoomDelta(zoomDelta);

  // Zoom to point: Keep the world coords under pinch position the same before
  // and after the zoom
  float postZoomWorldX, postZoomWorldY;
  mCamera.normWindowToWorldCoords(pinchX, pinchY, postZoomWorldX,
                                  postZoomWorldY);
  Vec2 deltaWorld = {postZoomWorldX - preZoomWorldX,
                     postZoomWorldY - preZoomWorldY};
  mCamera.setPanDelta(deltaWorld);
}

void EventHandler::panEventMouse(int x, int y) {
  int deltaX = (mCamera.windowSize().width / 2) + (x - mMouseButtonDownX),
      deltaY = (mCamera.windowSize().height / 2) + (y - mMouseButtonDownY);

  float deviceX;
  float deviceY;
  mCamera.windowToDeviceCoords(deltaX, deltaY, deviceX, deviceY);

  Vec2 pan = {mCamera.basePan().x + (deviceX / mCamera.zoom()),
              mCamera.basePan().y +
                  (deviceY / mCamera.zoom() / mCamera.aspect())};
  mCamera.setPan(pan);
}

void EventHandler::panEventFinger(float x, float y) {
  float deltaX = 0.5f + (x - mFingerDownX), deltaY = 0.5f + (y - mFingerDownY);

  float deviceX;
  float deviceY;
  mCamera.normWindowToDeviceCoords(deltaX, deltaY, deviceX, deviceY);

  Vec2 pan = {mCamera.basePan().x + (deviceX / mCamera.zoom()),
              mCamera.basePan().y +
                  (deviceY / mCamera.zoom() / mCamera.aspect())};
  mCamera.setPan(pan);
}

void EventHandler::processEvents() {
  // Handle events.
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      std::terminate();
      break;
    }

    case SDL_WINDOWEVENT: {
      if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
          event.window.event == SDL_WINDOWEVENT_RESIZED) {
        if (event.window.windowID == mWindowID) {
          int width = event.window.data1;
          int height = event.window.data2;
          SetViewport(width, height);
        }
      }
      break;
    }

    case SDL_MOUSEWHEEL: {
      SDL_MouseWheelEvent *m = reinterpret_cast<SDL_MouseWheelEvent *>(&event);
      bool mouseWheelDown = (m->y < 0);
      zoomEventMouse(mouseWheelDown, mMousePositionX, mMousePositionY);
      break;
    }

    case SDL_MOUSEMOTION: {
      SDL_MouseMotionEvent *m =
          reinterpret_cast<SDL_MouseMotionEvent *>(&event);
      mMousePositionX = m->x;
      mMousePositionY = m->y;
      if (mMouseButtonDown && !mFingerDown && !mPinch) {
        panEventMouse(mMousePositionX, mMousePositionY);
      }
      break;
    }

    case SDL_MOUSEBUTTONDOWN: {
      SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent *)&event;
      if (m->button == SDL_BUTTON_LEFT && !mFingerDown && !mPinch) {
        mMouseButtonDown = true;
        mMouseButtonDownX = m->x;
        mMouseButtonDownY = m->y;
        mCamera.setBasePan();
      }
      break;
    }

    case SDL_MOUSEBUTTONUP: {
      SDL_MouseButtonEvent *m =
          reinterpret_cast<SDL_MouseButtonEvent *>(&event);
      if (m->button == SDL_BUTTON_LEFT)
        mMouseButtonDown = false;
      break;
    }

    case SDL_FINGERMOTION: {
      if (mFingerDown) {
        SDL_TouchFingerEvent *m =
            reinterpret_cast<SDL_TouchFingerEvent *>(&event);

        // Finger down and finger moving must match.
        if (m->fingerId == mFingerDownId)
          panEventFinger(m->x, m->y);
      }
      break;
    }

    case SDL_FINGERDOWN: {
      if (!mPinch) {
        // Finger already down means multiple fingers, which is handled by.
        // multigesture event.
        if (mFingerDown) {
          mFingerDown = false;
        } else {
          SDL_TouchFingerEvent *m =
              reinterpret_cast<SDL_TouchFingerEvent *>(&event);

          mFingerDown = true;
          mFingerDownX = m->x;
          mFingerDownY = m->y;
          mFingerDownId = m->fingerId;
          mCamera.setBasePan();
        }
      }
      break;
    }

    case SDL_MULTIGESTURE: {
      SDL_MultiGestureEvent *m =
          reinterpret_cast<SDL_MultiGestureEvent *>(&event);
      if (m->numFingers == 2 && std::abs(m->dDist) >= cPinchZoomThreshold) {
        mPinch = true;
        mFingerDown = false;
        mMouseButtonDown = false;
        zoomEventPinch(m->dDist, m->x, m->y);
      }
      break;
    }

    case SDL_FINGERUP: {
      mFingerDown = false;
      mPinch = false;
      break;
    }
    }

#ifdef EVENTS_DEBUG
    printf("event=%d mousePos=%d,%d mouseButtonDown=%d fingerDown=%d pinch=%d "
           "aspect=%f window=%dx%d\n",
           event.type, mMousePositionX, mMousePositionY, mMouseButtonDown,
           mFingerDown, mPinch, mCamera.aspect(), mCamera.windowSize().width,
           mCamera.windowSize().height);
    printf("    zoom=%f pan=%f,%f\n", mCamera.zoom(), mCamera.pan()[0],
           mCamera.pan()[1]);
#endif
  }
}
