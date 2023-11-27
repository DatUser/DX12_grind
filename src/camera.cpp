#include "camera.h"

Camera::Camera()
: m_oPos(DEFAULT_CAMERA_POS),
  m_oUp(DEFAULT_CAMERA_UP),
  m_oForward(Vec3(0.f, 1.f, 0.f)),
  m_fFov(DEFAULT_CAMERA_FOV),
  m_fNearPlane(DEFAULT_CAMERA_NEAR),
  m_fFarPlane(DEFAULT_CAMERA_FAR),
  m_fAspectRatio(DEFAULT_CAMERA_ASPECT_RATIO)
{
}