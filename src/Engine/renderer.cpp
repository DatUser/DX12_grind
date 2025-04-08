#include "Engine/renderer.h"

#include "app.h"

#include "Core/Core.h"

#include "Engine/Mesh.h"
#include "Engine/render_buffers.h"
#include "Engine/scene.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"
#include "RHI/rhi_shader.h"

#define TO_SHADER_TYPE(x) std::integral_constant<EShaderStage, x>{}

// Creates, compiles and stores shader
#define INIT_RENDERER_SHADER(x)\
	m_mapShaders[static_cast<unsigned int>(x)] = RHI::GetInterface()->CreateShader(x);	\
	m_mapShaders[static_cast<unsigned int>(x)]->Compile();

std::unique_ptr<Renderer> Renderer::m_spInstance{new Renderer{}};

Renderer::Renderer()
: m_mapShaders({nullptr})
, m_spScene(std::make_unique<Scene>())
, m_spCurrentViewport(nullptr)
, m_spConstantBuffer(std::make_shared<ConstantBuffers>())
, m_spConstantBufferResource(nullptr)
{
	InitResources();
	InitShaders();

	InitTestScene();
}

void Renderer::InitResources()
{
	RHI::GetInterface()->CreateSwapchain(App::GetInstance()->GetMainWindow()->GetHandle());

	m_spConstantBufferResource = RHI::GetInterface()->CreateBuffer(
		&m_spConstantBuffer,
		sizeof(ConstantBuffers),
		ERHIBufferFlags::CONSTANT,
		ERHICPUAccessFlags::WRITE,
		ERHIBufferUsage::DYNAMIC);

	m_spCurrentViewport = RHI::GetInterface()->CreateViewport(600, 600);
}

void Renderer::InitShaders()
{
	INIT_RENDERER_SHADER(ERendererShaders::FORWARD_VS)
	INIT_RENDERER_SHADER(ERendererShaders::FORWARD_PS)
}

void Renderer::Tick()
{
	GenerateFrame();
	PresentFrame();
}

void Renderer::GenerateFrame()
{
	//Add passes for every drawable instances in scene
	RHI::GetInterface()->ClearRenderView();

	UpdateConstantBuffers();

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
	//CPU update CBO

	RHI::GetInterface()->SetBuffer(m_spConstantBufferResource.get(), TO_SHADER_TYPE(EShaderStage::VERTEX));
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
		UpdateMesh(pMesh.get());

		// Bind shader
		RHI::GetInterface()->SetVertexShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_VS)].get());
		RHI::GetInterface()->SetPixelShader(m_mapShaders[static_cast<unsigned int>(ERendererShaders::FORWARD_PS)].get());

		// Bind data
		RHI::GetInterface()->SetVertexBuffer(pMesh->GetVertexBuffer());
		RHI::GetInterface()->SetIndexBuffer(pMesh->GetIndexBuffer());

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}

void Renderer::InitTestScene()
{
	std::vector<Mesh*> vMeshes;
	load_obj("models/teapot.obj", vMeshes);
	Mesh* pMesh = vMeshes[0];

	m_spScene->AddMesh(std::shared_ptr<Mesh>{pMesh});
}