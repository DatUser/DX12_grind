#include "app.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    const LPCSTR pWinName = LPCSTR("Default");

    App app(pWinName);
    return app.Execute();
}