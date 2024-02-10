#pragma once

#define DEFAULT_CAMERA_POS              {0.f, 0.f, -10.f}
#define DEFAULT_CAMERA_UP               {0.f, 1.f, 0.f}
#define DEFAULT_CAMERA_FOV              90.f
#define DEFAULT_CAMERA_NEAR             1.f
#define DEFAULT_CAMERA_FAR              1000.f
// This one is temporary (until resolution is added there)
#define DEFAULT_CAMERA_ASPECT_RATIO     16.f/9.f

#include "fwdtypes.h"
#include "Core/Core.h"

class Camera
{
public:
    Camera();

    inline const Vec3& GetPosition() const { return m_oPos; }
    inline const Vec3& GetUpVector() const { return m_oUp; }

    inline const float GetFOV() const { return m_fFov; }

    inline const float GetNearClipping() const { return m_fNearPlane; }
    inline const float GetFarClipping() const { return m_fFarPlane; }

    inline const float GetAspectRatio() const { return m_fAspectRatio; }

private:
    Vec3 m_oPos;
    Vec3 m_oUp;
    Vec3 m_oForward;

    /**
     * @brief Horizontal FOV
     */
    float m_fFov;

    float m_fNearPlane;
    float m_fFarPlane;

    float m_fAspectRatio;
    // TODO: Add camera resolution
};