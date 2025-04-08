#include "Engine/Mesh.h"

#include "Core/Core.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"

namespace dx = DirectX;

Mesh::Mesh()
: m_vVertices()
, m_vIndices()
{
}

Mesh::Mesh(
	const std::vector<float>& vVertices,
	const std::vector<int>& vIndices,
	uint32_t uVertexOffset /* = 0 */,
	uint32_t uIndexOffset /* = 0 */
)
: m_oPos(Vec3(0.f, 0.f, 0.f))
, m_oModelMatrix(dx::XMMatrixIdentity())
, m_vVertices(vVertices)
, m_vIndices(vIndices)
, m_uVertexOffset(uVertexOffset)
, m_uIndexOffset(uIndexOffset)
{
	m_spVertexBuffer = RHI::GetInterface()->CreateBuffer(
		m_vVertices.data(),
		m_vVertices.size() * sizeof(float),
		ERHIBufferFlags::VERTEX
		);

	m_spIndiceBuffer = RHI::GetInterface()->CreateBuffer(
		m_vIndices.data(),
		m_vIndices.size() * sizeof(int),
		ERHIBufferFlags::INDEX
		);
}

Mesh::~Mesh()
{
}

void Mesh::UpdateModelMatrix()
{
	memcpy(&m_oModelMatrix.r[3], &m_oPos, sizeof(Vec3));
}