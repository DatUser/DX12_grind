#include "RHI/rhi_viewport.h"

#include "Input/controller.h"
#include "Engine/renderer.h"
#include "Engine/scene.h"

RHIViewport::RHIViewport()
: m_spMainCamera(std::make_shared<Camera>())
{
    Renderer::GetInterface()->GetScene()->AddCamera(std::move(m_spMainCamera));
    if (!Renderer::GetInterface()->GetScene()->GetController()->HasCamera())
        Renderer::GetInterface()->GetScene()->GetController()->SetControlledCam(m_spMainCamera);
}