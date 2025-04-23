#include "Input/mouse.h"

#include "Core/asserts.h"
#include "Core/Core.h"
#include "Engine/app.h"

Mouse::Mouse()
{
    POINT oCursorPos{};
    GetCursorPos(&oCursorPos);
    ScreenToClient(App::GetInstance()->GetMainWindow()->GetHandle(), &oCursorPos);

    m_nX = oCursorPos.x;
    m_nY = oCursorPos.y;
    m_nPrevX = m_nX;
    m_nPrevY = m_nY;
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
    m_nPrevX = m_nX;
    m_nPrevY = m_nY;

    m_nX = nX;
    m_nY = nY;
}