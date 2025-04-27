#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Core/Core.h"

// TODO: Create enunm for rendering state managemenent

class RHIBuffer;

class Mesh
{
	friend void load_obj(const std::string& path, std::vector<Mesh*>& arrMeshes);

public:
	~Mesh();

	Mesh(
		const std::vector<float>& vVertices,
		const std::vector<int>& vIndices,
		/* TODO: Move to submesh */
		uint32_t uVertexOffset = 0,
		uint32_t uIndexOffset = 0
	);

	void UpdateModelMatrix();

	//TODO: Move to .hpp
	void SetPosition(const Vec3& oPos)
	{
		m_oPos = oPos;
		UpdateModelMatrix();
	}

	const Vec3& GetPosition() const { return m_oPos; }
	//TODO : Handle rotation
	const Mat4x4& GetModelMatrix() const { return m_oModelMatrix; }

	void* GetVerticeData() { return m_vVertices.data(); }
	void* GetIndiceData() { return m_vIndices.data(); }

	size_t GetNumVerts() const { return m_vVertices.size() / 3; }
	size_t GetNumIndices() const { return m_vIndices.size(); }

	uint32_t GetVertexOffset() const { return m_uVertexOffset; }
	uint32_t GetIndexOffset() const { return m_uIndexOffset; }

	RHIBuffer* GetVertexBuffer() const { return m_spVertexBuffer.get(); }
	RHIBuffer* GetIndexBuffer() const { return m_spIndiceBuffer.get(); }

private:
	// Avoids to have Mesh with unitilized rendering resources
	Mesh(/* args */);

	Vec3 				m_oPos;
	Mat4x4				m_oModelMatrix;

	std::vector<float>	m_vVertices;
	std::vector<int>	m_vIndices;

	// Rendering resources
	std::shared_ptr<RHIBuffer>	m_spVertexBuffer;
	std::shared_ptr<RHIBuffer>	m_spIndiceBuffer;

	// TODO: Move to submesh
	uint32_t m_uVertexOffset;
	uint32_t m_uIndexOffset;
};