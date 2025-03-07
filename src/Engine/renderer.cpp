#include "Engine/renderer.h"

#include "Core/Core.h"

#include "Engine/Mesh.h"
#include "Engine/render_buffers.h"
#include "Engine/scene.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"


std::unique_ptr<Renderer> Renderer::m_spInstance{new Renderer{}};

Renderer::Renderer()
: m_spScene(std::make_unique<Scene>())
, m_spConstantBuffer(std::make_shared<ConstantBuffers>())
, m_spConstantBufferResource(nullptr)
{
}

void Renderer::Init()
{
	m_spConstantBufferResource = RHI::GetInterface()->CreateBuffer(
		&m_spConstantBuffer->oModelViewProj,
		sizeof(Mat4x4),
		ERHIBufferFlags::CONSTANT,
		ECPUAccessFlags::WRITE);
}

void Renderer::Tick()
{
	GenerateFrame();
}


void Renderer::GenerateFrame()
{
	//Add passes for every drawable instances in scene
	RHI::GetInterface()->ClearRenderView();

	UpdateConstantBuffers();

	Pass_Forward();

	RHI::GetInterface()->Draw();
}

void Renderer::UpdateConstantBuffers()
{
	RHI::GetInterface()->SetBuffer(m_spConstantBufferResource.get());
}

void Renderer::UpdateMesh(Mesh *pMesh)
{
}

void Renderer::DrawMesh(Mesh *pMesh)
{
}

void Renderer::Pass_Forward()
{
	for (auto&& pMesh : m_spScene->GetMeshes())
	{
		UpdateMesh(pMesh.get());

		RHI::GetInterface()->SetVertexBuffer(pMesh->GetVertexBuffer());
		RHI::GetInterface()->SetIndexBuffer(pMesh->GetIndexBuffer());

		DrawMesh(pMesh.get());
		//RHI::GetInterface()->Draw();
	}
}
void Renderer::InitTestScene()
{
}