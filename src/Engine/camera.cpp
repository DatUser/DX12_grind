#include "Engine/camera.h"

namespace dx = DirectX;

Camera::Camera()
: m_oFocusPoint(DEFAULT_FOCUS_POINT)
, m_fFov(DEFAULT_CAMERA_VFOV)
, m_fNearPlane(DEFAULT_CAMERA_NEAR)
, m_fFarPlane(DEFAULT_CAMERA_FAR)
, m_fAspectRatio(DEFAULT_CAMERA_ASPECT_RATIO)
, m_fFocusDistance(DEFAULT_CAMERA_FOCUS_DISTANCE)
{
    //dx::XMStoreFloat4(&m_oRotation, dx::XMQuaternionIdentity());
    m_oTransform.SetPosition(DEFAULT_CAMERA_POS);
}

void Camera::UpdateFocusPoint()
{
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(&m_oTransform.GetPosition());
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_oTransform.GetForward());

    dx::XMVECTOR oTranslation = dx::XMVectorScale(oCamFw, m_fFocusDistance);
    dx::XMVECTOR oFocusPoint = dx::XMVectorAdd(oCamPos, oTranslation);

    dx::XMStoreFloat3(&m_oFocusPoint, oFocusPoint);
}