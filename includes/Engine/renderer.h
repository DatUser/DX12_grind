#pragma once

class Renderer
{
public:
	void Tick();

private:
	Renderer();
	~Renderer();

	void GenerateFrame();
	void Pass_Forward();

	static std::unique_ptr<Scene> m_spInstance();

	std::unique_ptr<Scene> m_spScene();
};
