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

void Transform::Translate(const Vec3 &oVec, float fScale)
{
    dx::XMVECTOR oPos = dx::XMLoadFloat3(m_oPos);
    dx::XMVECTOR oDir = dx::XMLoadFloat3(&oVec);

    oDir = dx::XMVectorScale(oDir, fScale);
    dx::XMVECTOR oNewPos = dx::XMVectorAdd(oPos, oDir);

    dx::XMStoreFloat3(m_oPos, oNewPos);
}

void Transform::UpdateMatrixRotation()
{
    //dx::XMVECTOR oShiftRot = dx::XMQuaternionRotationRollPitchYaw(fPitch, fYaw, 0.f);
    //oCamRot = dx::XMQuaternionMultiply(oCamRot, oShiftRot);
    //oCamRot = dx::XMQuaternionNormalize(oCamRot);


    //dx::XMStoreFloat4(&m_oRotation, oRot); TODO: use Quaternions instead of pitch yaw roll
    UnitVectorsFromYawPitchRoll(m_oRot, *m_oUp, *m_oForward, *m_oRight);
}

void Transform::UnitVectorsFromYawPitchRoll(const Rot &oRot, Vec3 &oUp, Vec3 &oForward, Vec3 &oRight)
{
    dx::XMVECTOR oXMRot = dx::XMQuaternionRotationRollPitchYaw(oRot.x, oRot.y, /*oRot.z*/0.f);

    dx::XMVECTOR oXMFw = dx::XMVector3Rotate(dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), oXMRot);
    dx::XMVECTOR oXMUp = dx::XMVector3Rotate(dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), oXMRot);
    dx::XMVECTOR oXMRight = dx::XMVector3Rotate(dx::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), oXMRot);

    dx::XMStoreFloat3(&oForward, oXMFw);
    dx::XMStoreFloat3(&oUp, oXMUp);
    dx::XMStoreFloat3(&oRight, oXMRight);
}
