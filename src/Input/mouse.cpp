#include "Input/mouse.h"

#include "Core/asserts.h"
#include "Core/Core.h"
#include "Engine/app.h"

Mouse::Mouse()
: m_nX(0)
, m_nY(0)
, m_nPrevX(0)
, m_nPrevY(0)
, m_cState(0)
, m_bIsValid(false)
{
}

Mouse::~Mouse()
{}

void Mouse::OnKeyPressed(Window*, unsigned int uKeyCode)
{
    m_cState |= uKeyCode;
}

void Mouse::OnKeyReleased(Window*, unsigned int uKeyCode)
{
    m_cState &= ~uKeyCode;
}

void Mouse::OnMouseMove(Window*, int nX, int nY)
{
    m_nPrevX = m_bIsValid ? m_nX : nX;
    m_nPrevY = m_bIsValid ? m_nY : nY;

    m_nX = nX;
    m_nY = nY;

    m_bIsValid = true;
}