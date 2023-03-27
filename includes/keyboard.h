#pragma once

#include <vector>

// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define NUM_WIN_KEYCODES 0xFF

enum EKeyState
{
    RELEASED = 0,
    PRESSED
};

class Keyboard
{
    friend class Window;

public:
    Keyboard(bool bUseAutoRepeat=false);
    ~Keyboard() = default;
    Keyboard(const Keyboard& oKb) = delete;

    void FlushPressedKeys();

    // GETTERS
    inline bool HasAutoRepeat() const { return m_bUseAutoRepeat; }
    inline EKeyState GetKeyState(unsigned int uKeyCode) const { return m_vKeyStates[uKeyCode]; }

    // SETTERS
    inline void SetAutoRepeat(bool bUseAutoRepeat) { m_bUseAutoRepeat = bUseAutoRepeat; }

protected:
    virtual void OnKeyPressed(unsigned int uKeyCode);
    virtual void OnKeyReleased(unsigned int uKeyCode);


    std::vector<EKeyState> m_vKeyStates;
    bool m_bUseAutoRepeat;
};