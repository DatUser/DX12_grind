#include "window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    const LPCSTR pWinName = LPCSTR("Default");
    Window win(pWinName, 600, 600);

    MSG uMsg;
    BOOL wbMsgResult;

    while ((wbMsgResult = GetMessage(&uMsg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&uMsg);
        DispatchMessage(&uMsg);
    }

    return (wbMsgResult == -1) ? -1 : uMsg.wParam;
}