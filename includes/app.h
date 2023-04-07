#pragma once

#include <chrono>

#include "window.h"

class App
{
public:
    App(LPCSTR pWinName);
    ~App();

    int Execute();

    inline double GetExecutionTime();



private:
    Window* m_pMainWindow;
    std::chrono::time_point<std::chrono::steady_clock> m_oStartTime;
};

#include "app.hpp"