#pragma once

#include <memory>

class Mesh;
class RHIBuffer;
class Scene;
struct ConstantBuffers;

class Renderer
{
	friend std::unique_ptr<Renderer>::deleter_type;
public:
	void Tick();

	inline static auto&& GetInterface()
	{
		return m_spInstance;
	}

private:
	Renderer();
	~Renderer() = default;

	/**
	 * @brief Initialiazes graphics resources of renderer
	 *
	 */
	void InitResources();

	void GenerateFrame();
	void UpdateConstantBuffers();
	/**
	 * @brief Update positions buffer(and maybe other stuff)
	 */
	void UpdateMesh(Mesh* pMesh);
	void DrawMesh(Mesh* pMesh);

	void Pass_Forward();

    /**
     * @brief Inits the default shader program
     */
    void InitTestScene();

	static std::unique_ptr<Renderer> m_spInstance;

	std::unique_ptr<Scene> m_spScene;
	std::shared_ptr<ConstantBuffers> 	m_spConstantBuffer;			// CPU data of constant buffer (MVP matrix)
	std::shared_ptr<RHIBuffer> 			m_spConstantBufferResource;
};
