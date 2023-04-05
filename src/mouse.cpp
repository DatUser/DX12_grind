#include "mouse.h"

Mouse::Mouse()
:   m_nX(0)
,   m_nY(0)
{}

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
    m_nX = nX;
    m_nY = nY;
}