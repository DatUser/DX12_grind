#pragma once
#include <array>

class RHITexture;

enum class EMaterialTexType :uint8_t
{
	DIFFUSE,
	NORMAL,
	ROUGHNESS,
	METALLIC,
	AMBIENT,
	HEIGHT,

	_size
};

class Material
{
public:
	Material() = default;
	~Material() = default;

	void SetTexture(EMaterialTexType eType, std::shared_ptr<RHITexture>&& pTexture)
	{
		m_arrTextures[static_cast<uint8_t>(eType)] = std::move(pTexture);
	}

private:
	std::array<std::shared_ptr<RHITexture>, static_cast<uint8_t>(EMaterialTexType::_size)>   m_arrTextures;
};