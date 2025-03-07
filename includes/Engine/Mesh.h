#pragma once

#include <vector>
#include <string>
#include <memory>

// TODO: Create enunm for rendering state managemenent

class RHIBuffer;

class Mesh
{
	friend void load_obj(const std::string& path, std::vector<Mesh*>& arrMeshes);

private:
	/* data */
public:
	Mesh(/* args */);
	~Mesh();

	Mesh(const std::vector<float>& vVertices, const std::vector<int>& vIndices);

	void* GetVerticeData() { return m_vVertices.data(); }
	void* GetIndiceData() { return m_vIndices.data(); }

	size_t GetNumVerts() const { return m_vVertices.size() / 3; }
	size_t GetNumIndices() const { return m_vIndices.size(); }

	RHIBuffer* GetVertexBuffer() const { return m_spVertexBuffer.get(); }
	RHIBuffer* GetIndexBuffer() const { return m_spIndiceBuffer.get(); }

private:
	std::vector<float>	m_vVertices;
	std::vector<int>	m_vIndices;

	// Rendering resources
	std::shared_ptr<RHIBuffer>	m_spVertexBuffer;
	std::shared_ptr<RHIBuffer>	m_spIndiceBuffer;
};
