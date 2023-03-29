#pragma once

enum EMouseState
{
    NONE,
    LB_PRESSED,
    LB_RELEASED,
    MB_PRESSED,
    MB_RELEASED,
    RB_PRESSED,
    RB_RELEASED,
    WHEEL_UP,
    WHEEL_DOWN
};

class Mouse
{
public:
    Mouse();
    ~Mouse();


    //GETTERS
    inline std::pair<int, int> GetMousePos();
    inline int GetXPos() { return m_nX; }
    inline int GetYPos() { return m_nY; }

private:
    int m_nX;
    int m_nY;
};