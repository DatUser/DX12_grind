#include "Engine/app.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    const LPCSTR pWinName = LPCSTR("Default");

	return App::GetInstance()->Execute();
    //App app(pWinName);
    //return app.Execute();
}