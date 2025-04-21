#pragma once

#include <array>
#include <memory>
#include <unordered_map>

#include "Engine/window.h"

class Camera;

enum class EKeyboardLayout : uint8_t
{
    AZERTY,
    QWERTY
};

enum class EDirections : uint8_t
{
    FORWARD,
    LEFT,
    RIGHT,
    BACKWARD
};

class Controller
{
public:
    Controller();
    Controller(const std::shared_ptr<Camera>& spCamera);
    ~Controller()  = default;

    void HandleMovementInput(Window*, unsigned int uKeyCode, EInputType eInputType);

    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();

    bool HasCamera() const { return m_spControlledCam.get() != nullptr; }
    void SetControlledCam(const std::shared_ptr<Camera> &spCam) { m_spControlledCam = spCam; }

    static bool IsAzertyLayout();
private:
    std::unordered_map<unsigned int, void (Controller::*)()> m_mapInputActions;
    std::shared_ptr<Camera>     m_spControlledCam;
    float                       m_fMoveSpeed;
};