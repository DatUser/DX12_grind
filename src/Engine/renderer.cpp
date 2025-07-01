#include "Engine/renderer.h"

#include "Engine/app.h"
#include "Engine/camera.h"

#include "Core/Core.h"
#include "Engine/light.h"

#include "Engine/mesh.h"
#include "Engine/renderer_ressources.h"
#include "Engine/render_buffers.h"
#include "Engine/scene.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"
#include "RHI/rhi_shader.h"
#include "RHI/rhi_swapchain.h"
#include "RHI/rhi_texture.h"
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
	// Viewport Stuff
	m_spCurrentViewport = RHI::GetInterface()->CreateViewport(App::GetInstance()->GetMainWindow()->GetHandle(), 600, 600);
	//RHI::GetInterface()->CreateSwapchain(App::GetInstance()->GetMainWindow()->GetHandle());

	// Pipeline setup
	RHI::GetInterface()->SetViewport(m_spCurrentViewport.get());
	//RHI::GetInterface()->SetRasterizerState(ECullMode::NONE, true);
	RHI::GetInterface()->SetRasterizerState(ECullMode::CULL_BACK, false);

	// Constant buffers
	const static Vec3 m_oFocus{0., 0., 0.};
	Camera* pCam = m_spCurrentViewport->GetCamera();

	//CPU update CBO
	Mat4x4 oView = pCam->ComputeViewMatrix();
	Mat4x4 oProj = pCam->ComputeProjectionMatrix();

	// View Projection data
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

	INIT_RENDERER_SHADER(ERendererShaders::GEOMETRY_VS)
	INIT_RENDERER_SHADER(ERendererShaders::GEOMETRY_PS)

	INIT_RENDERER_SHADER(ERendererShaders::LIGHT_CS)

	m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::FORWARD)] =
		RHI::GetInterface()->CreateTexture(nullptr,
			m_spCurrentViewport->GetWidth(),
			m_spCurrentViewport->GetHeight(),
			ETextureFormat::R8G8B8A8_UNORM,
			ERHITextureFlags::SHADER_RESOURCE | ERHITextureFlags::RENDER_TARGET);

	m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_ALBEDO)] =
		RHI::GetInterface()->CreateTexture(nullptr,
			m_spCurrentViewport->GetWidth(),
			m_spCurrentViewport->GetHeight(),
			ETextureFormat::R8G8B8A8_UNORM,
			ERHITextureFlags::SHADER_RESOURCE | ERHITextureFlags::RENDER_TARGET);

	m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_NORMAL)] =
		RHI::GetInterface()->CreateTexture(nullptr,
			m_spCurrentViewport->GetWidth(),
			m_spCurrentViewport->GetHeight(),
			ETextureFormat::R32G32B32A32_FLOAT,
			ERHITextureFlags::SHADER_RESOURCE | ERHITextureFlags::RENDER_TARGET);

	m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_POS)] =
		RHI::GetInterface()->CreateTexture(nullptr,
			m_spCurrentViewport->GetWidth(),
			m_spCurrentViewport->GetHeight(),
			ETextureFormat::R32G32B32A32_FLOAT,
			ERHITextureFlags::SHADER_RESOURCE | ERHITextureFlags::RENDER_TARGET);

	m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::LIGHTS)] =
		RHI::GetInterface()->CreateTexture(nullptr,
			m_spCurrentViewport->GetWidth(),
			m_spCurrentViewport->GetHeight(),
			ETextureFormat::R8G8B8A8_UNORM,
			ERHITextureFlags::SHADER_RESOURCE | ERHITextureFlags::UNORDERED_ACCESS);
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
	RHI::GetInterface()->ClearDepthStencilView(m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());
	RHI::GetInterface()->ClearRenderView(m_spCurrentViewport->GetSwapchain()->GetBackBufferRTV(), 1.f, 0.f, 0.f, 1.f);

	// Graphic passes
	Pass_Geometry();

	// Unset render target
	RHI::GetInterface()->SetContextRenderTarget(nullptr, nullptr);

	// Compute Passes
	Pass_Lights();

	// Copy to final render target
	RHI::GetInterface()->CopyTexture(
		//m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_ALBEDO)].get(),
		m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::LIGHTS)].get(),
		m_spCurrentViewport->GetSwapchain()->GetBackBufferRTV());

	// Todo : Add Render target to be filled and displayed
	RHI::GetInterface()->SetContextRenderTarget(
		m_spCurrentViewport->GetSwapchain()->GetBackBufferRTV(),
		m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());

	//RHI::GetInterface()->Draw();
}

void Renderer::PresentFrame()
{
	m_spCurrentViewport->GetSwapchain()->Present();
}

void Renderer::UpdateConstantBuffers()
{
	Camera* pCam = m_spCurrentViewport->GetCamera();

	//CPU update CBO
	//const Mat4x4& oView = pCam->GetTransform().GetMatrix();
	Mat4x4 oView = pCam->ComputeViewMatrix();
	Mat4x4 oProj = pCam->ComputeProjectionMatrix();

	m_spConstantBuffer->oView = (oView);
	m_spConstantBuffer->oProj = (oProj);
	m_spConstantBuffer->oViewProj = dx::XMMatrixTranspose(oView * oProj);
	//m_spConstantBuffer->oViewProj = m_spConstantBuffer->oView * m_spConstantBuffer->oProj;

	// GPU update
	RHI::GetInterface()->SetBufferData(m_spConstantBufferResource.get(), m_spConstantBuffer.get());
}

void Renderer::UpdateMesh(mesh *pMesh)
{
	// Check if mesh render state is dirty and update buffers if so

	// Update Model view if needed
}

void Renderer::DrawMesh(mesh *pMesh)
{
	RHI::GetInterface()->DrawIndexed(pMesh->GetNumIndices(), pMesh->GetIndexOffset(), pMesh->GetVertexOffset());
}

void Renderer::Pass_Forward()
{
	RHI::GetInterface()->ClearDepthStencilView(m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());
	RHI::GetInterface()->ClearRenderView(m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::FORWARD)].get(), 1.f, 0.f, 0.f, 1.f);

	RHI::GetInterface()->ClearShaders();

	for (auto&& pMesh : m_spScene->GetMeshes())
	{
		// Wrong place to do this
		//UpdateMesh(pMesh.get());

		// Bind shader
		RHI::GetInterface()->SetVertexShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_VS)].get());
		RHI::GetInterface()->SetGeometryShader(/*m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_GS)].get()*/nullptr);
		RHI::GetInterface()->SetPixelShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_PS)].get());

		// Bind data
		RHI::GetInterface()->SetVertexBuffer(pMesh->GetVertexBuffer());
		RHI::GetInterface()->SetIndexBuffer(pMesh->GetIndexBuffer());
		RHI::GetInterface()->SetBuffer(
			RendererRessources::uViewProjCB,
			m_spConstantBufferResource.get(),
			TO_SHADER_TYPE(EShaderStage::VERTEX)
			);
		//RHI::GetInterface()->SetBuffer(m_spConstantBufferResource.get(), TO_SHADER_TYPE(EShaderStage::GEOMETRY));

		// Bind output
		RHI::GetInterface()->SetDepthStencilState(m_spCurrentViewport->GetSwapchain());
		RHI::GetInterface()->SetContextRenderTarget(
			m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::FORWARD)].get(),
			m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}

void Renderer::Pass_DebugNormals(const RHITexture* pTarget)
{
	RHI::GetInterface()->ClearShaders();
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
		RHI::GetInterface()->SetBuffer(
			RendererRessources::uViewProjCB,
			m_spConstantBufferResource.get(),
			TO_SHADER_TYPE(EShaderStage::VERTEX)
			);
		RHI::GetInterface()->SetBuffer(
			RendererRessources::uViewProjCB,
			m_spConstantBufferResource.get(),
			TO_SHADER_TYPE(EShaderStage::GEOMETRY)
			);

		RHI::GetInterface()->SetContextRenderTarget(
			pTarget,
			m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());

		// Bind output
		RHI::GetInterface()->SetDepthStencilState(m_spCurrentViewport->GetSwapchain());
		RHI::GetInterface()->SetContextRenderTarget(
			pTarget,
			m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}

void Renderer::Pass_Geometry()
{
	RHI::GetInterface()->ClearDepthStencilView(m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());
	RHI::GetInterface()->ClearRenderView(m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_POS)].get(), 1.f, 0.f, 0.f, 1.f);
	RHI::GetInterface()->ClearRenderView(m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_NORMAL)].get(), 1.f, 0.f, 0.f, 1.f);
	RHI::GetInterface()->ClearRenderView(m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_ALBEDO)].get(), 1.f, 0.f, 0.f, 0.f);

	RHI::GetInterface()->ClearShaders();

	for (auto&& pMesh : m_spScene->GetMeshes())
	{
		// Wrong place to do this
		//UpdateMesh(pMesh.get());

		// Bind shader
		RHI::GetInterface()->SetVertexShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::GEOMETRY_VS)].get());
		RHI::GetInterface()->SetPixelShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::GEOMETRY_PS)].get());

		// Bind data
		RHI::GetInterface()->SetVertexBuffer(pMesh->GetVertexBuffer());
		RHI::GetInterface()->SetIndexBuffer(pMesh->GetIndexBuffer());
		RHI::GetInterface()->SetBuffer(
			RendererRessources::uViewProjCB,
			m_spConstantBufferResource.get(),
			TO_SHADER_TYPE(EShaderStage::VERTEX)
			);

		// Bind output
		RHI::GetInterface()->SetDepthStencilState(m_spCurrentViewport->GetSwapchain());
		RHITexture* pRTVs[3] = {
			m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_NORMAL)].get(),
			m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_POS)].get(),
			m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_ALBEDO)].get()
		};
		RHI::GetInterface()->SetContextRenderTargets(
			pRTVs,
			3,
			m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}

void Renderer::Pass_Lights()
{
	RHI::GetInterface()->ClearDepthStencilView(m_spCurrentViewport->GetSwapchain()->GetDepthStencilView());
	RHI::GetInterface()->ClearUnorderedAccessView(
		m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::LIGHTS)].get(),
		1.f, 0.f, 0.f, 1.f);

	RHI::GetInterface()->ClearShaders();

	for (auto&& pLight : m_spScene->GetLights())
	{
		// TODO: Set compute shader
		RHI::GetInterface()->SetComputeShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::LIGHT_CS)].get());

		// Bind data
		// Set light data
		RHI::GetInterface()->SetBufferData(m_spLightBufferResource.get(), pLight.get());
		RHI::GetInterface()->SetBuffer(
				RendererRessources::uLightCB,
				m_spLightBufferResource.get(),
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
			);

		// Set data from Gbuffer
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferAlbedoSRV,
				m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_ALBEDO)].get(),
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
				);
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferNormalsSRV,
				m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_NORMAL)].get(),
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
		);
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferPosSRV,
				m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::GBUFFER_POS)].get(),
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
			);

		// Bind output
		RHI::GetInterface()->SetTexture(
				RendererRessources::uLightsUAV,
				m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::LIGHTS)].get(),
				TO_SHADER_TYPE(EShaderStage::COMPUTE),
				true
			);

		// Computes light
		RHI::GetInterface()->Dispatch(m_mapPassRenderTargets[static_cast<unsigned int>(ERendererPassesRT::LIGHTS)].get());

		// Unsets views
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferAlbedoSRV,
				nullptr,
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
				);
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferNormalsSRV,
				nullptr,
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
			);
		RHI::GetInterface()->SetTexture(
				RendererRessources::uGBufferPosSRV,
				nullptr,
				TO_SHADER_TYPE(EShaderStage::COMPUTE)
			);

		RHI::GetInterface()->SetTexture(
				RendererRessources::uLightsUAV,
				nullptr,
				TO_SHADER_TYPE(EShaderStage::COMPUTE),
				true
			);
	}
}

void Renderer::InitTestScene()
{
	std::vector<mesh*> vMeshes;
	load_obj("models/sphere.obj", vMeshes);
	mesh* pMesh = vMeshes[0];

	m_spScene->AddMesh(std::shared_ptr<mesh>{pMesh});

	m_spScene->AddLight(std::make_shared<Light>(Light{
		{0.0f, 5.0f, -6.0f},
		1.f,
		{0.1f, 0.7f, 0.2f, 1.}
	}));

	// Light data
	m_spLightBufferResource = RHI::GetInterface()->CreateBuffer(
		m_spScene->GetLights()[0].get(),
		sizeof(Light),
		ERHIBufferFlags::CONSTANT,
		ERHICPUAccessFlags::WRITE,
		ERHIBufferUsage::DYNAMIC);
}