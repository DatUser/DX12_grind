#pragma once

#include "Core/Core.h"
#include <optional>
#include <sstream>

#include "Input/keyboard.h"
#include "Input/inputevent.h"
#include "Input/mouse.h"
#include "Input/moveevent.h"

class RHI;

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

	void CreateEvents();

    std::optional<int> ProcessMessage();

	void LockMousePosition();
	void UpdateCenterPosition();

    // GETTERS
	inline HWND GetHandle() const { return m_hWnd; }
    inline InputEvent* GetInputEvent() { return m_pInputEvent; }
	inline MoveEvent* GetMoveEvent() { return m_pMoveEvent; }

	inline const POINTS& GetSize() const { return m_oWindowSize; }
	inline const POINT& GetCenter() const { return m_oRelativeCenter; }
	inline const POINT& GetCenterInScreen() const { return m_oAbsoluteCenter; }

	inline int GetCursorPosX() const { return m_pMouse->GetXPos(); }
	inline int GetCursorPosY() const { return m_pMouse->GetYPos(); }

	inline int GetPrevCursorPosX() const { return m_pMouse->GetPrevXPos(); }
	inline int GetPrevCursorPosY() const { return m_pMouse->GetPrevYPos(); }

	inline uint32_t GetWidth() const { return m_oWindowSize.x; }
	inline uint32_t GetHeight() const { return m_oWindowSize.y; }

    // SETTER
    inline void SetTitle(LPCSTR pTitle)   { SetWindowText(m_hWnd, pTitle); }

private:
    static LRESULT CALLBACK RegHandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    static LRESULT CALLBACK AcceptMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

    LRESULT CALLBACK HandleMsg(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam);

	Quat	m_oRotation;
	POINTS 	m_oWindowSize;
	POINT	m_oAbsoluteCenter; // Center pixel of the window (in the whole screen)
	POINT	m_oRelativeCenter; // Center pixel of the window (in the whole screen)
    HWND 	m_hWnd;

    Keyboard* 	m_pKeyboard;
    Mouse* 		m_pMouse;

    InputEvent* 	m_pInputEvent;
    MoveEvent* 		m_pMoveEvent;
	SizeEvent* 		m_pSizeEvent;
};

void OnWindowResize(Window* pSender, int nWidth, int nHeight);

//DEBUG PURPOSE
void OnInputEvent(Window* pSender, unsigned int uKeyCode, EInputType eType);
void OnMouseMove(Window* pSender, int nX, int nY);