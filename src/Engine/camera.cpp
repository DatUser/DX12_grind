#include "Engine/camera.h"

namespace dx = DirectX;

Camera::Camera()
: m_oPos(DEFAULT_CAMERA_POS)
, m_oUp(DEFAULT_CAMERA_UP)
, m_oForward(DEFAULT_CAMERA_FWD)
, m_oRight(DEFAULT_CAMERA_RIGHT)
, m_fPitch(0.f)
, m_fYaw(0.f)
, m_fRoll(0.f)
, m_oFocusPoint(DEFAULT_FOCUS_POINT)
, m_fFov(DEFAULT_CAMERA_VFOV)
, m_fNearPlane(DEFAULT_CAMERA_NEAR)
, m_fFarPlane(DEFAULT_CAMERA_FAR)
, m_fAspectRatio(DEFAULT_CAMERA_ASPECT_RATIO)
, m_fFocusDistance(DEFAULT_CAMERA_FOCUS_DISTANCE)
{
    dx::XMStoreFloat4(&m_oRotation, dx::XMQuaternionIdentity());
}

void Camera::UpdateFocusPoint()
{
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(&m_oPos);
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_oForward);

    dx::XMVECTOR oTranslation = dx::XMVectorScale(oCamFw, m_fFocusDistance);
    dx::XMVECTOR oFocusPoint = dx::XMVectorAdd(oCamPos, oTranslation);

    dx::XMStoreFloat3(&m_oFocusPoint, oFocusPoint);
}