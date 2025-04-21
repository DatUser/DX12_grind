#pragma once

#include <memory>
#include <vector>

class Controller;
class Mesh;
class Camera;

class Scene
{
public:
	// TODO: Add viewport to attach scene to as argument
	Scene();
	~Scene();

	inline const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const
	{
		return m_vMeshes;
	}

	inline const std::vector<std::shared_ptr<Camera>>& GetCameras() const
	{
		return m_vCameras;
	}

	inline Controller* GetController() const { return m_spController.get(); }

	inline void AddMesh(std::shared_ptr<Mesh>&& spMesh)
	{
		m_vMeshes.push_back(spMesh);
	}

	inline void AddCamera(std::shared_ptr<Camera>&& spCamera)
	{
		m_vCameras.push_back(spCamera);
	}

private:
	std::vector<std::shared_ptr<Mesh>>		m_vMeshes;
	std::vector<std::shared_ptr<Camera>>	m_vCameras;

	std::unique_ptr<Controller>				m_spController;
};