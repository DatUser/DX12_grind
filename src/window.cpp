#include "window.h"

Window::WindowClass Window::WindowClass::m_Wc;

Window::WindowClass::WindowClass()
: m_hInstance(GetModuleHandle(nullptr))
{
    //window class
    WNDCLASSEX WndClassEx;
    ZeroMemory(&WndClassEx, sizeof(WndClassEx));
    WndClassEx.cbSize = sizeof(WndClassEx);
    WndClassEx.style = CS_OWNDC;
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
    //Adjust window size to hold content
    RECT rect;
    rect.left = 200;
    rect.right = rect.left + nWidth;
    rect.top = 200;
    rect.bottom = rect.bottom + nHeight;

    if (!FAILED(AdjustWindowRect(&rect, WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU, FALSE)))
    {
        m_hWnd = CreateWindow(WindowClass::GetName(), pWinName,
            WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
            200, 200, 600, 600, nullptr, nullptr,
            WindowClass::GetInstance(),
            this);

        if (m_hWnd)
            ShowWindow(m_hWnd, SW_SHOW);
        else
            LOG_LAST_ERROR();

    }
    else
        LOG_LAST_ERROR();
}

Window::~Window()
{
    DestroyWindow(m_hWnd);
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

void OnKeyPressed(WPARAM wKeyCode)
{
    std::stringstream stream;
    stream << "Pressed " << (char) wKeyCode << " key!" << std::endl;
    OutputDebugString(stream.str().c_str());
}

void OnKeyReleased(WPARAM wKeyCode)
{
    std::stringstream stream;
    stream << "Released " << (char) wKeyCode << " key!" << std::endl;
    OutputDebugString(stream.str().c_str());
}

void OnMouseMove(LPARAM lMousePos)
{
    //Retrieve Mouse Pos: Px Pos relative to window
    POINTS ptMousePos = MAKEPOINTS(lMousePos);

    std::stringstream stream;
    stream << "Mouse pos is: (" << ptMousePos.x << ", " << ptMousePos.y << ")" << std::endl;
    OutputDebugString(stream.str().c_str());
}

LRESULT CALLBACK Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        OnKeyPressed(wParam);
        break;
    case WM_KEYUP:
        OnKeyReleased(wParam);
        break;
    case WM_MOUSEMOVE:
        OnMouseMove(lParam);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}