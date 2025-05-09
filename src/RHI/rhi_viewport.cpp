#include "RHI/rhi_viewport.h"

#include "Input/controller.h"
#include "Engine/renderer.h"
#include "Engine/scene.h"

#include "RHI/rhi_swapchain.h"

RHIViewport::RHIViewport(HWND hWnd, uint32_t uWidth, uint32_t uHeight)
: m_spMainCamera(std::make_shared<Camera>())
, m_spSwapchain(RHI::GetInterface()->CreateSwapchain(hWnd, uWidth, uHeight))
{
    Renderer::GetInterface()->GetScene()->AddCamera(std::move(m_spMainCamera));
    if (!Renderer::GetInterface()->GetScene()->GetController()->HasCamera())
        Renderer::GetInterface()->GetScene()->GetController()->SetControlledCam(m_spMainCamera);
}