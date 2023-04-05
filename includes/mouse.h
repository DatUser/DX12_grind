#pragma once

#include <utility>

#include "inputevent.h"

enum EMouseState
{
    LB_PRESSED = 1,
    MB_PRESSED = 2,
    RB_PRESSED = 4
};

class Window;

/**
 * @brief Keeps track of mouse position and mouse buttons state
 *
 */
class Mouse
{
public:
    Mouse();
    ~Mouse();

    void OnKeyPressed(Window* pSender, unsigned int uKeyCode);
    void OnKeyReleased(Window* pSender, unsigned int uKeyCode);
    void OnMouseMove(Window* pSender, int nX, int nY);

    //GETTERS
    inline std::pair<int, int> GetMousePos() const { return { m_nX, m_nY }; }
    inline int GetXPos() const { return m_nX; }
    inline int GetYPos() const { return m_nY; }
    inline bool IsPressed(EMouseState eState) { return eState&m_cState; }

private:
    int m_nX;
    int m_nY;

    uint8_t m_cState;
};