#pragma once

#include <Windows.h>
#include <comdef.h>
#include <sstream>

#include "keyboard.h"

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

private:
    static LRESULT CALLBACK RegHandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    static LRESULT CALLBACK AcceptMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    LRESULT CALLBACK HandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    HWND m_hWnd;

    Keyboard* m_pKeyboard;
};