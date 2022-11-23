//
// Camera - pan, zoom, and window resizing
//
#include "camera.h"

#include <SDL2/SDL_opengles2.h>
#include <algorithm>

Camera::Camera()
    : mCameraUpdated(false), mWindowSize({}), mViewport({}), cZoomMin(0.1f),
      cZoomMax(10.0f), mBasePan({0.0f, 0.0f}), mPan({0.0f, 0.0f}), mZoom(1.0f),
      mAspect(1.0f) {
  setWindowSize(640, 480);
}

bool Camera::updated() {
  if (mCameraUpdated) {
    mCameraUpdated = false;
    return true;
  }

  return false;
}

void Camera::setWindowSize(int width, int height) {
  mWindowSize = {width, height};
  mViewport = {static_cast<GLfloat>(width), static_cast<GLfloat>(height)};
  setAspect(width / static_cast<GLfloat>(height));
  mCameraUpdated = true;
}

// Clamp val between lo and hi
float Camera::clamp(float val, float lo, float hi) {
  return std::max(lo, std::min(val, hi));
}

// Convert from normalized window coords (x,y) in ([0.0, 1.0], [1.0, 0.0]) to
// device coords ([-1.0, 1.0], [-1.0,1.0])
void Camera::normWindowToDeviceCoords(float normWinX, float normWinY,
                                      float &deviceX, float &deviceY) {
  deviceX = (normWinX - 0.5f) * 2.0f;
  deviceY = (1.0f - normWinY - 0.5f) * 2.0f;
}

// Convert from window coords (x,y) in ([0, mWindowWidth], [mWindowHeight, 0])
// to device coords ([-1.0, 1.0], [-1.0,1.0])
void Camera::windowToDeviceCoords(int winX, int winY, float &deviceX,
                                  float &deviceY) {
  normWindowToDeviceCoords(winX / static_cast<float>(mWindowSize.width),
                           winY / static_cast<float>(mWindowSize.height),
                           deviceX, deviceY);
}

// Convert from device coords ([-1.0, 1.0], [-1.0,1.0]) to world coords ([-inf,
// inf], [-inf, inf])
void Camera::deviceToWorldCoords(float deviceX, float deviceY, float &worldX,
                                 float &worldY) {
  worldX = deviceX / mZoom - mPan.x;
  worldY = deviceY / mAspect / mZoom - mPan.y;
}

// Convert from window coords (x,y) in ([0, windowWidth], [windowHeight, 0]) to
// world coords ([-inf, inf], [-inf, inf])
void Camera::windowToWorldCoords(int winX, int winY, float &worldX,
                                 float &worldY) {
  float deviceX, deviceY;
  windowToDeviceCoords(winX, winY, deviceX, deviceY);
  deviceToWorldCoords(deviceX, deviceY, worldX, worldY);
}

// Convert from normalized window coords (x,y) in in ([0.0, 1.0], [1.0, 0.0]) to
// world coords ([-inf, inf], [-inf, inf])
void Camera::normWindowToWorldCoords(float normWinX, float normWinY,
                                     float &worldX, float &worldY) {
  float deviceX, deviceY;
  normWindowToDeviceCoords(normWinX, normWinY, deviceX, deviceY);
  deviceToWorldCoords(deviceX, deviceY, worldX, worldY);
}
