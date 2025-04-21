#include "Engine/window.h"

#include <iostream>

#include "Core/asserts.h"
#include "RHI/rhi.h"
#include "RHI/fwd_rhi.h"
#include "D3D11/D3D11Interface.h"

//#define DEBUG_INPUT
//#define DEBUG_SIZE

//Window::WindowClass Window::WindowClass::m_Wc;

void OnWindowResize(Window*, int nWidth, int nHeight)
{
	std::stringstream stream;
	stream << "Size: " << nWidth << ", " << nHeight << std::endl;
	OutputDebugString(stream.str().c_str());
}

void OnKeyPressed(unsigned int uKeyCode)
{
	std::stringstream stream;
	stream << "Pressed " << (char) uKeyCode << " key!" << std::endl;
	OutputDebugString(stream.str().c_str());
}

void OnKeyReleased(unsigned int uKeyCode)
{
    std::stringstream stream;
    stream << "Released " << (char) uKeyCode << " key!" << std::endl;
    OutputDebugString(stream.str().c_str());
}

void OnInputEvent(Window* pSender, unsigned int uKeyCode, EInputType eType)
{
	if (eType == EInputType::KEY_RELEASED)
		OnKeyReleased(uKeyCode);
	else
		OnKeyPressed(uKeyCode);
}

void OnMouseMove(Window* pSender, int nX, int nY)
{
    //Retrieve Mouse Pos: Px Pos relative to window
    //POINTS ptMousePos = MAKEPOINTS(lMousePos);

    std::stringstream stream;
    stream << "Mouse pos is: (" << nX << ", " << nY << ")" << std::endl;
    OutputDebugString(stream.str().c_str());
    std::cout << "Mouse pos is: (" << nX << ", " << nY << ")" << std::endl;
}

Window::WindowClass::WindowClass()
: m_hInstance(GetModuleHandle(nullptr))
{
    //window class
    WNDCLASSEX WndClassEx;
    ZeroMemory(&WndClassEx, sizeof(WndClassEx));
    WndClassEx.cbSize = sizeof(WndClassEx);
    WndClassEx.style = CS_OWNDC; //|CS_DBLCLKS;
    WndClassEx.lpfnWndProc = &Window::RegHandleMsg;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = 0;
    WndClassEx.hInstance = m_hInstance;
    WndClassEx.hIcon = nullptr;
    WndClassEx.hCursor = nullptr;
    WndClassEx.hbrBackground = nullptr;
    WndClassEx.lpszMenuName = nullptr;
    WndClassEx.lpszClassName = GetName();
    WndClassEx.hIconSm = nullptr;

    //Register window class
    RegisterClassEx(&WndClassEx);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), m_hInstance);
}

Window::Window(LPCSTR pWinName, int nWidth, int nHeight)
{
    m_pKeyboard = new Keyboard();
    m_pMouse = new Mouse();
    m_pInputEvent = new InputEvent();
    m_pMoveEvent = new MoveEvent();
	m_pSizeEvent = new SizeEvent();

    if (!m_pKeyboard || !m_pMouse || !m_pInputEvent || !m_pMoveEvent)
        return;

    #ifdef DEBUG_INPUT
    m_pInputEvent->AddAction(&OnInputEvent);
    m_pMoveEvent->AddAction(&OnMouseMove);
    #endif
	#ifdef DEBUG_SIZE
	m_pSizeEvent->AddAction(&OnWindowResize);
	#endif

    //Adjust window size to hold content
    RECT rect;
    rect.left = 200;
    rect.right = rect.left + nWidth;
    rect.top = 200;
    rect.bottom = rect.bottom + nHeight;

    if (AdjustWindowRect(&rect, WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU, FALSE) != 0)
    {
		//const WindowClass& oWinClass{WindowClass::GetInstance()};
        m_hWnd = CreateWindow(WindowClass::GetName(), pWinName,
            WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU|WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            //200, 200,
            600, 600, nullptr, nullptr,
            WindowClass::GetHInstance(),
            this);

        if (m_hWnd)
        {
            ShowWindow(m_hWnd, SW_SHOW);
			CreateInterface();
            //m_pDxGraphics = CreateInterface(m_hWnd, new Camera());
        }
        else
            LOG_LAST_ERROR();

    }
    else
        LOG_LAST_ERROR();
}

Window::~Window()
{
    delete m_pKeyboard;
    delete m_pMouse;
    delete m_pInputEvent;
    delete m_pMoveEvent;
	delete m_pSizeEvent;
    DestroyWindow(m_hWnd);
    //delete m_pDxGraphics;
}

LRESULT CALLBACK Window::RegHandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam)
{
    //Handle pre window creation
    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

        //Gets instance address passed in CreateWindow function
        Window* pWin = (Window*) pCreate->lpCreateParams;

        //Register actual HandleMsg
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::AcceptMsg));

        //Register actual Window ptr
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWin));
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::AcceptMsg(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam)
{
    Window* pWin = (Window*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return pWin->HandleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDBLCLK:
        {
            //Register state
            m_pMouse->OnKeyPressed(this, wParam);

            //Broadcast event
            m_pInputEvent->Broadcast(this, wParam, EInputType::KEY_DOUBLE_CLICK);
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        {
            //Register state
            m_pMouse->OnKeyPressed(this, wParam);

            //Broadcast event
            m_pInputEvent->Broadcast(this, wParam, EInputType::KEY_PRESSED);
        }
        break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        {
            //Register state
            m_pMouse->OnKeyReleased(this, wParam);

            //Broadcast event
            m_pInputEvent->Broadcast(this, wParam, EInputType::KEY_RELEASED);
        }
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (!(lParam&0x40000000) || m_pKeyboard->HasAutoRepeat())
        {
            //Register state
            m_pKeyboard->OnKeyPressed(this, wParam);

            //Broadcast event
            m_pInputEvent->Broadcast(this, wParam, EInputType::KEY_PRESSED);
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        {
            //Register state
            m_pKeyboard->OnKeyReleased(this, wParam);

            //Broadcast event
            m_pInputEvent->Broadcast(this, wParam, EInputType::KEY_RELEASED);
        }
        break;
    case WM_MOUSEMOVE:
        {
            POINTS ptMousePos = MAKEPOINTS(lParam);

            //Register pos
            m_pMouse->OnMouseMove(this, ptMousePos.x, ptMousePos.y);

            //Broadcast event
            m_pMoveEvent->Broadcast(this, ptMousePos.x, ptMousePos.y);

            LockMousePosition();
        }
        break;
    case WM_KILLFOCUS:
        m_pKeyboard->FlushPressedKeys();
	case WM_SIZE:
		m_oWindowSize = MAKEPOINTS(lParam);

		// Broadcast event
		m_pSizeEvent->Broadcast(this, m_oWindowSize.x, m_oWindowSize.y);
		break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

std::optional<int> Window::ProcessMessage()
{
    MSG uMsg;

    while (PeekMessage(&uMsg, nullptr, 0, 0, PM_REMOVE))
    {
        if (uMsg.message == WM_QUIT)
            return uMsg.wParam;
        TranslateMessage(&uMsg);
        DispatchMessage(&uMsg);
    }

    return {};
}

void Window::LockMousePosition()
{
    if (GetFocus() == m_hWnd)
        SetCursorPos(m_oAbsoluteCenter.x, m_oAbsoluteCenter.y);
}

void Window::UpdateCenterPosition()
{
    static RECT oRect{};
    GetClientRect(m_hWnd, &oRect);

    m_oAbsoluteCenter.x = (oRect.right - oRect.left) / 2;
    m_oAbsoluteCenter.y = (oRect.bottom - oRect.top) / 2;

    ClientToScreen(m_hWnd, &m_oAbsoluteCenter);
}