//
// Camera - pan, zoom, and window resizing
//

#pragma once

#include <GLES2/gl2.h>

struct Rect {
  int width;
  int height;
};

struct Vec2 {
  GLfloat x;
  GLfloat y;
};

class Camera {
public:
  Camera();
  bool updated();

  Rect &windowSize() { return mWindowSize; }
  void setWindowSize(int width, int height);
  GLfloat *viewport() { return reinterpret_cast<GLfloat *>(&mViewport); }

  GLfloat *pan() { return reinterpret_cast<GLfloat *>(&mPan); }
  GLfloat zoom() { return mZoom; }
  GLfloat aspect() { return mAspect; }

  void setPan(Vec2 pan) {
    mPan = pan;
    mCameraUpdated = true;
  }
  void setPanDelta(Vec2 panDelta) {
    mPan.x += panDelta.x;
    mPan.y += panDelta.y;
    mCameraUpdated = true;
  }
  void setZoom(GLfloat zoom) {
    mZoom = clamp(zoom, cZoomMin, cZoomMax);
    mCameraUpdated = true;
  }
  void setZoomDelta(GLfloat zoomDelta) {
    mZoom = clamp(mZoom + zoomDelta, cZoomMin, cZoomMax);
    mCameraUpdated = true;
  }
  void setAspect(GLfloat aspect) {
    mAspect = aspect;
    mCameraUpdated = true;
  }

  Vec2 &basePan() { return mBasePan; }
  void setBasePan() { mBasePan = mPan; }

  void normWindowToDeviceCoords(float normWinX, float normWinY, float &deviceX,
                                float &deviceY);
  void windowToDeviceCoords(int winX, int winY, float &deviceX, float &deviceY);
  void deviceToWorldCoords(float deviceX, float deviceY, float &worldX,
                           float &worldY);
  void windowToWorldCoords(int winX, int winY, float &worldX, float &worldY);
  void normWindowToWorldCoords(float normWinX, float normWinY, float &worldX,
                               float &worldY);

private:
  float clamp(float val, float lo, float hi);

  bool mCameraUpdated;
  Rect mWindowSize;
  Vec2 mViewport;
  const GLfloat cZoomMin;
  const GLfloat cZoomMax;
  Vec2 mBasePan;
  Vec2 mPan;
  GLfloat mZoom;
  GLfloat mAspect;
};
