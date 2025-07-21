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
    Material();
    ~Material();

private:
    std::array<RHITexture, static_cast<uint8_t>(EMaterialTexType::_size)>   m_arrTextures;
};