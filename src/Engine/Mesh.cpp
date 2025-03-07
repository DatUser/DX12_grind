#include "Engine/Mesh.h"

#include "RHI/rhi.h"
#include "RHI/rhi_buffer.h"

Mesh::Mesh()
: m_vVertices()
, m_vIndices()
{
}

Mesh::Mesh(const std::vector<float>& vVertices, const std::vector<int>& vIndices)
: m_vVertices(m_vVertices)
, m_vIndices(m_vIndices)
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