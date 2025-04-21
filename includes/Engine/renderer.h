#pragma once

#include <array>
#include <memory>

struct ConstantBuffers;
class Mesh;
class Scene;

class RHIBuffer;
class RHIShader;
class RHIViewport;

enum class ERendererShaders : uint8_t
{
	FORWARD_VS,
	FORWARD_PS,

	_size
};

class Renderer
{
	friend std::unique_ptr<Renderer>::deleter_type;
public:
	void Initialize();

	void Tick();

	inline Scene* GetScene() const { return m_spScene.get(); };

	inline RHIViewport* GetCurrentViewport() const { return m_spCurrentViewport.get(); };

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

	/**
	 * @brief Initialiazes default renderer shaders
	 *
	 */
	void InitShaders();

	void GenerateFrame();
	void PresentFrame();

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

	std::array<std::shared_ptr<RHIShader>, static_cast<unsigned int>(ERendererShaders::_size)> m_mapShaders;	// TODO: Move to submesh ERendererShaders::_size> m_mapShaders;
	std::unique_ptr<Scene> 				m_spScene;
	std::shared_ptr<RHIViewport> 		m_spCurrentViewport;
	std::shared_ptr<ConstantBuffers> 	m_spConstantBuffer;			// CPU data of constant buffer (MVP matrix)
	std::shared_ptr<RHIBuffer> 			m_spConstantBufferResource;
};