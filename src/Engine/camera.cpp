#include "Engine/camera.h"

Camera::Camera()
: m_oPos(DEFAULT_CAMERA_POS),
  m_oUp(DEFAULT_CAMERA_UP),
  m_oForward(DEFAULT_CAMERA_FWD),
  m_oRight(DEFAULT_CAMERA_RIGHT),
  m_fFov(DEFAULT_CAMERA_VFOV),
  m_fNearPlane(DEFAULT_CAMERA_NEAR),
  m_fFarPlane(DEFAULT_CAMERA_FAR),
  m_fAspectRatio(DEFAULT_CAMERA_ASPECT_RATIO)
{
}