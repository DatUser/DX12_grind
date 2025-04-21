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

#include "Core/fwdtypes.h"
#include "Core/Core.h"

class Camera
{
    friend class Controller;
public:
    Camera();

    inline const Vec3& GetPosition() const { return m_oPos; }
    inline void SetPosition(const Vec3& oPos) { m_oPos = oPos; }

    inline const Vec3& GetUpVector() const { return m_oUp; }
    inline const Vec3& GetForwardVector() const { return m_oForward; }
    inline const Vec3& GetRightVector() const { return m_oRight; }

    inline const Vec3& GetFocusPoint() const { return m_oFocusPoint; }

    inline const float GetFOV() const { return m_fFov; }

    inline const float GetNearClipping() const { return m_fNearPlane; }
    inline const float GetFarClipping() const { return m_fFarPlane; }

    inline const float GetAspectRatio() const { return m_fAspectRatio; }

private:
    void UpdateFocusPoint();

    Vec3 m_oPos;
    Vec3 m_oUp;
    Vec3 m_oForward;
    Vec3 m_oRight;

    Quat    m_oRotation;
    float   m_fPitch;
    float   m_fYaw;
    float   m_fRoll;

    Vec3 m_oFocusPoint;

    /**
     * @brief VFOV in radians
     */
    float m_fFov;

    float m_fNearPlane;
    float m_fFarPlane;

    float m_fAspectRatio;
    float m_fFocusDistance;
};