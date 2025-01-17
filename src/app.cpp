#include "app.h"
#include "RHI/rhi.h"

// TODO: Set a random name in compile time
std::unique_ptr<App> App::m_spAppInstance{new App("Default")};
//std::unique_ptr<App> App::m_spAppInstance{std::make_unique<App>("Default")};

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
        //continue;
        //m_pMainWindow->SetTitle(std::to_string(GetExecutionTime()).c_str());

        RetrieveFrame();
    }

    return oOptOutputMsg.value();
}

void App::RetrieveFrame()
{
    m_pMainWindow->GetRHI()->ClearRenderView();
    m_pMainWindow->GetRHI()->Draw();
    m_pMainWindow->GetRHI()->PresentFrame();
}