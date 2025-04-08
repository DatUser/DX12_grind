#pragma once

#include <memory>
#include <vector>

class Mesh;
class Camera;

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

	inline const std::vector<std::shared_ptr<Camera>>& GetCameras() const
	{
		return m_vCameras;
	}

	inline void AddMesh(std::shared_ptr<Mesh>&& spMesh)
	{
		m_vMeshes.push_back(spMesh);
	}

private:
	std::vector<std::shared_ptr<Mesh>>		m_vMeshes;
	std::vector<std::shared_ptr<Camera>>	m_vCameras;
};
