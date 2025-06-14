#include "Input/keyboard.h"
#include "Engine/window.h"

Keyboard::Keyboard(bool bUseAutoRepeat)
:   m_vKeyStates(std::vector<EKeyState>(NUM_WIN_KEYCODES, EKeyState::RELEASED))
,   m_bUseAutoRepeat(bUseAutoRepeat)
{}

void Keyboard::FlushPressedKeys()
{
    std::memset(m_vKeyStates.data(), EKeyState::RELEASED, NUM_WIN_KEYCODES);
}

void Keyboard::OnKeyPressed(Window* pSender, unsigned int uKeyCode)
{
    m_vKeyStates[uKeyCode] = EKeyState::PRESSED;
}

void Keyboard::OnKeyReleased(Window*, unsigned int uKeyCode)
{
    m_vKeyStates[uKeyCode] = EKeyState::RELEASED;
}