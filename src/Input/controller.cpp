#include "Input/controller.h"

#include "Core/Core.h"
#include "Engine/app.h"

#include "Engine/camera.h"
#include "Engine/window.h"

namespace dx = DirectX;

Controller::Controller()
: m_spControlledCam(nullptr)
, m_fMoveSpeed(0.01f)
, m_fRotationSpeed(0.01f)
{
    m_mapInputActions['D'] = &Controller::MoveRight;
    m_mapInputActions['S'] = &Controller::MoveBackward;
    if (IsAzertyLayout())
    {
        m_mapInputActions['Z'] = &Controller::MoveForward;
        m_mapInputActions['Q'] = &Controller::MoveLeft;
    }
    else
    {
        m_mapInputActions['W'] = &Controller::MoveForward;
        m_mapInputActions['A'] = &Controller::MoveLeft;
    }
}

Controller::Controller(const std::shared_ptr<Camera>& spCamera)
: m_spControlledCam(spCamera)
, m_fMoveSpeed(0.01f)
, m_fRotationSpeed(0.01f)
{
    m_mapInputActions['D'] = &Controller::MoveRight;
    m_mapInputActions['S'] = &Controller::MoveBackward;
    if (IsAzertyLayout())
    {
        m_mapInputActions['Z'] = &Controller::MoveForward;
        m_mapInputActions['Q'] = &Controller::MoveLeft;
    }
    else
    {
        m_mapInputActions['W'] = &Controller::MoveForward;
        m_mapInputActions['A'] = &Controller::MoveLeft;
    }
}

void Controller::HandleMovementInput(Window*, unsigned int uKeyCode, EInputType eInputType)
{
    if (m_mapInputActions.find(uKeyCode) != m_mapInputActions.end())
    {
        // TODO: add sensivity
        // Move camera
        (this->*m_mapInputActions[uKeyCode])();
        m_spControlledCam->UpdateFocusPoint();
    }
}

void Controller::HandleRotationInput(Window*, int x, int y)
{
    const POINT& oWindowCenter = App::GetInstance()->GetMainWindow()->GetAbsoluteCenter();
    float fDeltaX = x - oWindowCenter.x;
    float fDeltaY = y - oWindowCenter.y;
    m_spControlledCam->m_fYaw += fDeltaX * m_fRotationSpeed;
    m_spControlledCam->m_fPitch = std::clamp(m_spControlledCam->m_fPitch + fDeltaY * m_fRotationSpeed, -1.5f, 1.5f);

    dx::XMVECTOR oCamRot = dx::XMQuaternionRotationRollPitchYaw(m_spControlledCam->m_fPitch, m_spControlledCam->m_fYaw, m_spControlledCam->m_fRoll);

    dx::XMVECTOR oCamFw = dx::XMVector3Rotate(dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), oCamRot);
    dx::XMVECTOR oCamUp = dx::XMVector3Rotate(dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), oCamRot);

    dx::XMStoreFloat4(&m_spControlledCam->m_oRotation, oCamRot);
    dx::XMStoreFloat3(&m_spControlledCam->m_oForward, oCamFw);
    dx::XMStoreFloat3(&m_spControlledCam->m_oUp, oCamUp);
}

void Controller::MoveForward()
{
    Vec3* pCamPos = &m_spControlledCam->m_oPos;
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(pCamPos);
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_spControlledCam->m_oForward);

    dx::XMVECTOR oNewPos = dx::XMVectorAdd(oCamPos, oCamFw);

    dx::XMStoreFloat3(pCamPos, oNewPos);
}

void Controller::MoveBackward()
{
    Vec3* pCamPos = &m_spControlledCam->m_oPos;
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(pCamPos);
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_spControlledCam->m_oForward);

    dx::XMVECTOR oNewPos = dx::XMVectorSubtract(oCamPos, oCamFw);

    dx::XMStoreFloat3(pCamPos, oNewPos);
}

void Controller::MoveLeft()
{
    Vec3* pCamPos = &m_spControlledCam->m_oPos;
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(pCamPos);
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_spControlledCam->m_oRight);

    dx::XMVECTOR oNewPos = dx::XMVectorSubtract(oCamPos, oCamFw);

    dx::XMStoreFloat3(pCamPos, oNewPos);
}

void Controller::MoveRight()
{
    Vec3* pCamPos = &m_spControlledCam->m_oPos;
    dx::XMVECTOR oCamPos = dx::XMLoadFloat3(pCamPos);
    dx::XMVECTOR oCamFw = dx::XMLoadFloat3(&m_spControlledCam->m_oRight);

    dx::XMVECTOR oNewPos = dx::XMVectorAdd(oCamPos, oCamFw);

    dx::XMStoreFloat3(pCamPos, oNewPos);
}

bool Controller::IsAzertyLayout()
{
    HKL keyboardLayout = GetKeyboardLayout(0);

    switch (LOWORD(keyboardLayout)) {
        case 0x040C:  // French (France)
        case 0x080C:  // French (Belgium)
        case 0x0C0C:  // French (Canada)
        case 0x100C:  // French (Switzerland)
        case 0x140C:  // French (Luxembourg)
            return true;
        default:
            return false;
    }
}