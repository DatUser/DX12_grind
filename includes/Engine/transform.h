#pragma once

#include "Core/Core.h"

struct Transform
{
    Transform();
    Transform(const Vec3& pPos, const Rot& oRot);

    void UpdateMatrixRotation();

    static void UnitVectorsFromYawPitchRoll(const Rot& oRot, Vec3& oUp, Vec3& oForward, Vec3& oRight);

    const Mat4x4& GetMatrix() const { return m_oMatrix; }

    void SetPosition(const Vec3& oPos) { *m_oPos = oPos; }

private:
    Mat4x4  m_oMatrix;

    Vec3    m_oRot;
    Vec3*   m_oPos;
    Vec3*   m_oUp;
    Vec3*   m_oForward;
    Vec3*   m_oRight;
};