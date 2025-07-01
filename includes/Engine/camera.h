#pragma once

#define DEFAULT_CAMERA_POS              {0.f, 0.f, -10.f}
#define DEFAULT_CAMERA_UP               {0.f, 1.f, 0.f}
#define DEFAULT_CAMERA_FWD              {0.f, 0.f, 1.f}
#define DEFAULT_CAMERA_RIGHT            {1.f, 0.f, 0.f}
#define DEFAULT_FOCUS_POINT              {0.f, 0.f, 0.f}
#define DEFAULT_CAMERA_VFOV              1.5708f // In radians (90 degrees)
#define DEFAULT_CAMERA_NEAR             1.f
#define DEFAULT_CAMERA_FAR              100000.f
// This one is temporary (until resolution is added there)
#define DEFAULT_CAMERA_ASPECT_RATIO     1.f
//#define DEFAULT_CAMERA_ASPECT_RATIO     16.f/9.f
#define DEFAULT_CAMERA_FOCUS_DISTANCE   10.f

#include "transform.h"
#include "Core/fwdtypes.h"
#include "Core/Core.h"

class Camera
{
    friend class Controller;
public:
    Camera();

    Mat4x4 ComputeViewMatrix() const {
        return DirectX::XMMatrixLookAtLH(
            DirectX::XMLoadFloat3(&m_oTransform.GetPosition()),
            DirectX::XMLoadFloat3(&m_oFocusPoint),
            DirectX::XMLoadFloat3(&m_oTransform.GetUp())
        );
    }

    Mat4x4 ComputeProjectionMatrix() const
    {
        return DirectX::XMMatrixPerspectiveFovLH(
            m_fFov,
            m_fAspectRatio,
            m_fNearPlane,
            m_fFarPlane
        );
    }

    const Transform& GetTransform() const { return m_oTransform; }
    Transform& GetTransform() { return m_oTransform; }

   const Mat4x4& GetViewMatrix() const { return m_oTransform.GetMatrix(); }

    inline const Vec3& GetFocusPoint() const { return m_oFocusPoint; }

    inline const float GetFOV() const { return m_fFov; }

    inline const float GetNearClipping() const { return m_fNearPlane; }
    inline const float GetFarClipping() const { return m_fFarPlane; }

    inline const float GetAspectRatio() const { return m_fAspectRatio; }

private:
    void UpdateFocusPoint();

    //Quat    m_oRotation;
    //Vec3 m_oPos;
    //Vec3 m_oUp;
    //Vec3 m_oForward;
    //Vec3 m_oRight;
    Transform m_oTransform;

    Vec3 m_oFocusPoint;

    //float   m_fPitch;
    //float   m_fYaw;
    //float   m_fRoll;

    /**
     * @brief VFOV in radians
     */
    float m_fFov;

    float m_fNearPlane;
    float m_fFarPlane;

    float m_fAspectRatio;
    float m_fFocusDistance;
};