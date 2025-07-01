#include "Input/controller.h"

#include <iostream>

#include "Core/Core.h"
#include "Engine/app.h"

#include "Engine/camera.h"
#include "Engine/window.h"

namespace dx = DirectX;

Controller::Controller()
: m_spControlledCam(nullptr)
, m_fMoveSpeed(0.1f)
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
, m_fMoveSpeed(0.1f)
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
    //const POINT& oWindowCenter = App::GetInstance()->GetMainWindow()->GetCenter();
    int iLastPosX = App::GetInstance()->GetMainWindow()->GetPrevCursorPosX();
    int iLastPosY = App::GetInstance()->GetMainWindow()->GetPrevCursorPosY();

    if (iLastPosX == x && iLastPosY == y)
        return;

    float fDeltaX = x - iLastPosX;
    float fDeltaY = y - iLastPosY;

    float fYaw = fDeltaX * m_fRotationSpeed;
    float fPitch = fDeltaY * m_fRotationSpeed;
    const Rot& oCamRot = m_spControlledCam->GetTransform().GetRotation();

    std::cout << "DeltaX: " << fDeltaX << ", DeltaY: " << fDeltaY << std::endl;
    std::cout << "YawShift: " << fYaw << "Pitch Shift: " << fPitch << std::endl;

    m_spControlledCam->GetTransform().SetRotationPitchYawRoll({oCamRot.x + fPitch, oCamRot.y + fYaw, 0.f});
    m_spControlledCam->UpdateFocusPoint();
}

void Controller::MoveForward()
{
    Transform& oCamTransform = m_spControlledCam->GetTransform();
    m_spControlledCam->GetTransform().Translate(oCamTransform.GetForward(), m_fMoveSpeed);
}

void Controller::MoveBackward()
{
    Transform& oCamTransform = m_spControlledCam->GetTransform();
    m_spControlledCam->GetTransform().Translate(oCamTransform.GetForward(), -m_fMoveSpeed);
}

void Controller::MoveLeft()
{
    Transform& oCamTransform = m_spControlledCam->GetTransform();
    m_spControlledCam->GetTransform().Translate(oCamTransform.GetRight(), -m_fMoveSpeed);
}

void Controller::MoveRight()
{
    Transform& oCamTransform = m_spControlledCam->GetTransform();
    m_spControlledCam->GetTransform().Translate(oCamTransform.GetRight(), m_fMoveSpeed);
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