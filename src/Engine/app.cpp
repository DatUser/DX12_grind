#include "Engine/app.h"

#include <functional>

#include "Engine/renderer.h"
#include "Engine/scene.h"

#include "Input//controller.h"

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

void App::Initialize()
{
    // Init Renderer
    Renderer::GetInterface()->Initialize();

    RegisterInputEvent();
    RegisterMouseMoveEvent();
    m_pMainWindow->UpdateCenterPosition();

}

void App::RegisterInputEvent()
{
    // Init inputs
    auto oInputLambda = [](Window* pWindow, unsigned int uKeyCode, EInputType eType)
    {
        Controller* pController = Renderer::GetInterface()->GetScene()->GetController();
        pController->HandleMovementInput(pWindow, uKeyCode, eType);
    };
    m_pMainWindow->GetInputEvent()->AddAction(oInputLambda);
}

void App::RegisterMouseMoveEvent()
{
    // Init Mouse movements
    auto oMouseLambda = [](Window* pWindow, int x, int y)
    {
        Controller* pController = Renderer::GetInterface()->GetScene()->GetController();
        pController->HandleRotationInput(pWindow, x, y);
    };
    m_pMainWindow->GetMoveEvent()->AddAction(oMouseLambda);
}

int App::Execute()
{
    std::optional<int> oOptOutputMsg;

    Initialize();

    while (!(oOptOutputMsg = m_pMainWindow->ProcessMessage()).has_value())
    {
        //continue;
        //m_pMainWindow->SetTitle(std::to_string(GetExecutionTime()).c_str());

		Renderer::GetInterface()->Tick();

        //RetrieveFrame();
    }

    return oOptOutputMsg.value();
}