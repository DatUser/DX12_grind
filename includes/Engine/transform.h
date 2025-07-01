#pragma once

#include "Core/Core.h"

struct Transform
{
    Transform();
    Transform(const Vec3& pPos, const Rot& oRot);

    void Translate(const Vec3& oVec, float fScale = 1.0f);

    static void UnitVectorsFromYawPitchRoll(const Rot& oRot, Vec3& oUp, Vec3& oForward, Vec3& oRight);

    const Mat4x4& GetMatrix() const { return m_oMatrix; }
    const Vec3& GetPosition() const { return *m_oPos; }
    const Rot& GetRotation() const { return m_oRot; }

    const Vec3& GetUp() const { return *m_oUp; }
    const Vec3& GetForward() const { return *m_oForward; }
    const Vec3& GetRight() const { return *m_oRight; }

    void SetPosition(const Vec3& oPos) { *m_oPos = oPos; }
    void SetRotationPitchYawRoll(const Rot& oRot)
    {
        m_oRot = oRot;
        UpdateMatrixRotation();
    }

private:
    void UpdateMatrixRotation();

    Mat4x4  m_oMatrix;

    Rot     m_oRot;
    Vec3*   m_oPos;
    Vec3*   m_oUp;
    Vec3*   m_oForward;
    Vec3*   m_oRight;
};