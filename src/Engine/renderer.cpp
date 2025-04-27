#include "Engine/renderer.h"

#include "Engine/app.h"
#include "Engine/camera.h"

#include "Core/Core.h"

#include "Engine/Mesh.h"
#include "Engine/render_buffers.h"
#include "Engine/scene.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"
#include "RHI/rhi_shader.h"
#include "RHI/rhi_viewport.h"

#define TO_SHADER_TYPE(x) std::integral_constant<EShaderStage, x>{}

// Creates, compiles and stores shader
#define INIT_RENDERER_SHADER(x)\
	m_mapShaders[static_cast<unsigned int>(x)] = RHI::GetInterface()->CreateShader(x);	\
	m_mapShaders[static_cast<unsigned int>(x)]->Compile();

namespace dx = DirectX;

std::unique_ptr<Renderer> Renderer::m_spInstance{new Renderer{}};

Renderer::Renderer()
: m_mapShaders({nullptr})
, m_spScene(std::make_unique<Scene>())
, m_spCurrentViewport(nullptr)
, m_spConstantBuffer(std::make_shared<ConstantBuffers>())
, m_spConstantBufferResource(nullptr)
{
}

void Renderer::Initialize()
{
	InitResources();
	InitShaders();

	InitTestScene();
}

void Renderer::InitResources()
{
	RHI::GetInterface()->CreateSwapchain(App::GetInstance()->GetMainWindow()->GetHandle());
	m_spCurrentViewport = RHI::GetInterface()->CreateViewport(600, 600);
	RHI::GetInterface()->SetViewport(m_spCurrentViewport.get());

	const static Vec3 m_oFocus{0., 0., 0.};
	Camera* pCam = m_spCurrentViewport->GetCamera();

	//CPU update CBO
	Mat4x4 oView = dx::XMMatrixLookAtLH(
	dx::XMLoadFloat3(&pCam->GetPosition()),
	dx::XMLoadFloat3(&m_oFocus),
	dx::XMLoadFloat3(&pCam->GetUpVector())
	);
	Mat4x4 oProj = dx::XMMatrixPerspectiveFovLH(
		pCam->GetFOV(),
		pCam->GetAspectRatio(),
		pCam->GetNearClipping(),
		pCam->GetFarClipping()
	);

	m_spConstantBuffer->oView = (oView);
	m_spConstantBuffer->oProj = (oProj);
	m_spConstantBuffer->oViewProj = dx::XMMatrixTranspose(oView * oProj);

	m_spConstantBufferResource = RHI::GetInterface()->CreateBuffer(
		m_spConstantBuffer.get(),
		sizeof(ConstantBuffers),
		ERHIBufferFlags::CONSTANT,
		ERHICPUAccessFlags::WRITE,
		ERHIBufferUsage::DYNAMIC);
}

void Renderer::InitShaders()
{
	INIT_RENDERER_SHADER(ERendererShaders::FORWARD_VS)
	INIT_RENDERER_SHADER(ERendererShaders::FORWARD_GS)
	INIT_RENDERER_SHADER(ERendererShaders::FORWARD_PS)
}

void Renderer::Tick()
{
	UpdateConstantBuffers();
	GenerateFrame();
	PresentFrame();
}

void Renderer::GenerateFrame()
{
	//Add passes for every drawable instances in scene
	RHI::GetInterface()->ClearRenderView();

	Pass_Forward();

	// Todo : Add Render target to be filled and displayed
	//RHI::GetInterface()->Draw();
}

void Renderer::PresentFrame()
{
	RHI::GetInterface()->PresentFrame();
}

void Renderer::UpdateConstantBuffers()
{
	Camera* pCam = m_spCurrentViewport->GetCamera();

	//CPU update CBO
	Mat4x4 oView = dx::XMMatrixLookAtLH(
		dx::XMLoadFloat3(&pCam->GetPosition()),
		dx::XMLoadFloat3(&pCam->GetFocusPoint()),
		dx::XMLoadFloat3(&pCam->GetUpVector())
		);
	Mat4x4 oProj = dx::XMMatrixPerspectiveFovLH(
		pCam->GetFOV(),
		pCam->GetAspectRatio(),
		pCam->GetNearClipping(),
		pCam->GetFarClipping()
	);

	m_spConstantBuffer->oView = (oView);
	m_spConstantBuffer->oProj = (oProj);
	m_spConstantBuffer->oViewProj = dx::XMMatrixTranspose(oView * oProj);
	//m_spConstantBuffer->oViewProj = m_spConstantBuffer->oView * m_spConstantBuffer->oProj;

	// GPU update
	RHI::GetInterface()->SetBufferData(m_spConstantBufferResource.get(), m_spConstantBuffer.get());
}

void Renderer::UpdateMesh(Mesh *pMesh)
{
	// Check if mesh render state is dirty and update buffers if so

	// Update Model view if needed
}

void Renderer::DrawMesh(Mesh *pMesh)
{
	RHI::GetInterface()->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetIndexOffset(), pMesh->GetVertexOffset());
}

void Renderer::Pass_Forward()
{
	for (auto&& pMesh : m_spScene->GetMeshes())
	{
		// Wrong place to do this
		//UpdateMesh(pMesh.get());

		// Bind shader
		RHI::GetInterface()->SetVertexShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_VS)].get());
		RHI::GetInterface()->SetGeometryShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_GS)].get());
		RHI::GetInterface()->SetPixelShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_PS)].get());

		// Bind data
		RHI::GetInterface()->SetVertexBuffer(pMesh->GetVertexBuffer());
		RHI::GetInterface()->SetIndexBuffer(pMesh->GetIndexBuffer());
		RHI::GetInterface()->SetBuffer(m_spConstantBufferResource.get(), TO_SHADER_TYPE(EShaderStage::VERTEX));

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}

void Renderer::InitTestScene()
{
	std::vector<Mesh*> vMeshes;
	load_obj("models/sphere.obj", vMeshes);
	Mesh* pMesh = vMeshes[0];

	m_spScene->AddMesh(std::shared_ptr<Mesh>{pMesh});
}