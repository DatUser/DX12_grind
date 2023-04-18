#pragma once

#include <Windows.h>
#include <comdef.h>
#include <optional>
#include <sstream>

#include "keyboard.h"
#include "inputevent.h"
#include "mouse.h"
#include "moveevent.h"

#define LOG_LAST_ERROR()                                                                    \
    {                                                                                       \
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());                                    \
        std::stringstream ss;                                                               \
        _com_error err(hr);                                                                 \
        ss <<  __FILE__ << ": " << __LINE__ << std::endl <<                                 \
            err.ErrorMessage() << std::endl;                                                \
        MessageBox(nullptr, reinterpret_cast<LPCSTR>(ss.str().c_str()), nullptr,            \
            MB_OK|MB_ICONERROR);                                                            \
        PostQuitMessage(hr);                                                                \
    }

class DXG;

class Window
{
public:
    class WindowClass
    {
    public:
        inline static HINSTANCE GetInstance() { return m_Wc.m_hInstance; };

        inline static LPCSTR GetName() { return "DefaultWindow"; }

    private:
        WindowClass();
        WindowClass(const WindowClass& oWc) = delete;
        ~WindowClass();

        HINSTANCE m_hInstance;

        static WindowClass m_Wc;
    };

    Window(LPCSTR pWinName, int nWidth, int nHeight);
    ~Window();

    std::optional<int> ProcessMessage();

    // GETTERS
    inline InputEvent* GetInputEvent() { return m_pInputEvent; }
    inline DXG* GetDXG() { return m_pDxGraphics; }

    // SETTER
    inline void SetTitle(LPCSTR pTitle)   { SetWindowText(m_hWnd, pTitle); }

private:
    static LRESULT CALLBACK RegHandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    static LRESULT CALLBACK AcceptMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    LRESULT CALLBACK HandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    HWND m_hWnd;

    Keyboard* m_pKeyboard;
    Mouse* m_pMouse;

    InputEvent* m_pInputEvent;
    MoveEvent* m_pMoveEvent;

    DXG* m_pDxGraphics;
};

//DEBUG PURPOSE
void OnInputEvent(Window* pSender, unsigned int uKeyCode, EInputType eType);
void OnMouseMove(Window* pSender, int nX, int nY);