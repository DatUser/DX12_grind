#include "app.h"

App::App(LPCSTR pWinName)
:   m_pMainWindow(new Window(pWinName, 600, 600))
,   m_oStartTime(std::chrono::steady_clock::now())
{
}

App::~App()
{
    delete m_pMainWindow;
}

int App::Execute()
{
    std::optional<int> oOptOutputMsg;
    while (!(oOptOutputMsg = m_pMainWindow->ProcessMessage()).has_value())
    {
        continue;
        //m_pMainWindow->SetTitle(std::to_string(GetExecutionTime()).c_str());
    }

    return oOptOutputMsg.value();
}