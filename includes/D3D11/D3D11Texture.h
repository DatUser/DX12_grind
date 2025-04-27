#pragma once

#include "RHI/rhi_texture.h"

class D3D11Texture : public RHITexture
{
public:
    D3D11Texture(
        void* pData,
        int iWidth,
        int iHeight,
        ETextureFormat eFormat
    );

    ~D3D11Texture() override = default;

private:
};