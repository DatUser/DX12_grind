#pragma once

#include "Core/Core.h"
#include <optional>
#include <sstream>

#include "keyboard.h"
#include "inputevent.h"
#include "mouse.h"
#include "moveevent.h"

class RHI;
class Camera;

class Window
{
public:
    class WindowClass
    {
    public:
		//inline static WindowClass& GetInstance() { return m_Wc; }
		//inline static const WindowClass& GetInstance() { return m_Wc; }

        inline static HINSTANCE GetHInstance()// const
		{
			static WindowClass m_Wc;
			return m_Wc.m_hInstance;
		};

        inline static LPCSTR GetName() { return "DefaultWindow"; }

    private:
        WindowClass();
        WindowClass(const WindowClass& oWc) = delete;
        ~WindowClass();

        HINSTANCE m_hInstance;

        //static WindowClass m_Wc;
    };

    Window(LPCSTR pWinName, int nWidth, int nHeight);
    ~Window();

    std::optional<int> ProcessMessage();

    // GETTERS
    inline InputEvent* GetInputEvent() { return m_pInputEvent; }
    //inline RHI* GetRHI() { return m_pDxGraphics; }

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

    //RHI* m_pDxGraphics;

    Camera* m_pCamera;
};

//DEBUG PURPOSE
void OnInputEvent(Window* pSender, unsigned int uKeyCode, EInputType eType);
void OnMouseMove(Window* pSender, int nX, int nY);