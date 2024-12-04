#pragma once

#include <vector>
#include <string>

// TODO: Create enunm for rendering state managemenent

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

private:
	std::vector<float>	m_vVertices;
	std::vector<int>	m_vIndices;
};
