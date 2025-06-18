#include "D3D11/D3D11Texture.h"

#include "Core/asserts.h"

D3D11Texture::D3D11Texture(
    void* pData,
    int iWidth,
    int iHeight,
    ETextureFormat eFormat,
    uint32_t uFlags
    )
: RHITexture(pData, iWidth, iHeight, eFormat, uFlags)
{
    ATLASSERT(D3D11Interface::GetInterface()->createTextureInternal(this) == S_OK);
    if (m_uFlags & ERHITextureFlags::RENDER_TARGET)
        ATLASSERT(D3D11Interface::GetInterface()->createRTVInternal(this) == S_OK);
    if (m_uFlags & ERHITextureFlags::UNORDERED_ACCESS)
        ATLASSERT(D3D11Interface::GetInterface()->createUAVInternal(this) == S_OK);
}

uint32_t D3D11Texture::CastToInterfaceBindFlags(uint32_t uFlags) {
    constexpr static std::array<uint32_t, 4> arrBindFlags{
        D3D11_BIND_SHADER_RESOURCE,
        D3D11_BIND_RENDER_TARGET,
        D3D11_BIND_DEPTH_STENCIL,
        D3D11_BIND_UNORDERED_ACCESS
    };

    uint32_t uBindFlags = 0;
    uint32_t uBit = 1;
    for (const uint32_t& uFlag : arrBindFlags)
    {
        if (uFlags & uBit)
            uBindFlags |= uFlag;
        uBit <<= 1;
    }

    return uBindFlags;
}