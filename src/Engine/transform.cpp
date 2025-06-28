#include "Engine/transform.h"

namespace dx = DirectX;

Transform::Transform()
: Transform(Vec3(0.f, 0.f, 0.f), Rot(0.f, 0.f, 0.f))
{
}

Transform::Transform(const Vec3& oPos, const Rot& oRot)
{
    // Init transform mat to default val
    m_oMatrix = dx::XMMatrixIdentity();

    // Bind unit vectors ptr to transform matrix data
    m_oRight = (Vec3*) &m_oMatrix.r[0];
    m_oUp = (Vec3*) &m_oMatrix.r[1];
    m_oForward = (Vec3*) &m_oMatrix.r[2];
    m_oPos = (Vec3*) &m_oMatrix.r[3];

    // Set translation
    *m_oPos = oPos;

    // Update rotation
    m_oRot = oRot;
    UpdateMatrixRotation();
}