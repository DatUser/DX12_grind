#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT oMsg, WPARAM wParam,
                        LPARAM lParam)
{
    switch (oMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, oMsg, wParam, lParam);
}

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
    WndClassEx.lpfnWndProc = WndProc;
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

    MSG oMsg;
    BOOL wbMsgResult;

    while ((wbMsgResult = GetMessage(&oMsg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&oMsg);
        DispatchMessage(&oMsg);
    }

    return (wbMsgResult == -1) ? -1 : oMsg.wParam;
}