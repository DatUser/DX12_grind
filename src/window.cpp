#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    const LPCSTR pClassName = LPCSTR("DefaultWindow");
    const LPCSTR pWinName = LPCSTR("Default");

    //window class
    WNDCLASSEX WndClassEx;
    ZeroMemory(&WndClassEx, sizeof(WndClassEx));
    WndClassEx.cbSize = sizeof(WndClassEx);
    WndClassEx.style = CS_OWNDC;
    WndClassEx.lpfnWndProc = DefWindowProc;
    WndClassEx.cbClsExtra = 0;
    WndClassEx.cbWndExtra = 0;
    WndClassEx.hInstance = hInstance;
    WndClassEx.hIcon = nullptr;
    WndClassEx.hCursor = nullptr;
    WndClassEx.hbrBackground = nullptr;
    WndClassEx.lpszMenuName = nullptr;
    WndClassEx.lpszClassName = pClassName;
    WndClassEx.hIconSm = nullptr;


    //Register window class
    RegisterClassEx(&WndClassEx);

    HWND hWnd = CreateWindowEx(
        0, pClassName, pWinName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        200, 200, 600, 600, nullptr, nullptr, hInstance, nullptr
    );

    ShowWindow(hWnd, SW_SHOW);

    while (true);

    return 0;
}