#pragma once

#include <array>
#include <dxgiformat.h>

#include "RHI/rhi_texture.h"

class D3D11Texture : public RHITexture
{
    friend class D3D11Interface;
public:
    D3D11Texture(
        void* pData,
        int iWidth,
        int iHeight,
        ETextureFormat eFormat,
        uint32_t uFlags
    );

    ~D3D11Texture() override = default;

    static DXGI_FORMAT CastToInterfaceFormat(ETextureFormat eFormat)
    {
        constexpr std::array<DXGI_FORMAT, static_cast<unsigned int>(ETextureFormat::_size)> arrFormats{
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT_R32G32_FLOAT,
            DXGI_FORMAT_R8_UNORM
        };

        return arrFormats[static_cast<unsigned int>(eFormat)];
    }

    static uint32_t CastToInterfaceBindFlags(uint32_t uFlags);

private:
    ComPtr<ID3D11Texture2D> m_spInitResource;
};