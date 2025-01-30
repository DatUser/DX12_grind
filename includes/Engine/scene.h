#pragma once

#include <memory>
#include <vector>

class Mesh;

class Scene
{
public:
	// TODO: Add viewport to attach scene to as argument
	Scene();
	~Scene() = default;

	inline const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const
	{
		return m_vMeshes;
	}

	inline void AddMesh(std::shared_ptr<Mesh>& spMesh)
	{
		m_vMeshes.push_back(spMesh);
	}

private:
	std::vector<std::shared_ptr<Mesh>>	m_vMeshes;
};
