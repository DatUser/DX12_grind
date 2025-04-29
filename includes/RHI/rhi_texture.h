#pragma once

#include <stdint.h>

#include "RHI/rhi_resource.h"

enum class ETextureFormat : uint8_t
{
    R8G8B8A8_UNORM,
    R32G32B32_FLOAT,
    R8_UNORM,

    _size
};

enum class ETextureFlags : uint32_t
{
    D3D11_BIND_SHADER_RESOURCE = 0x1u,
    D3D11_BIND_RENDER_TARGET = 0x20L,
    D3D11_BIND_DEPTH_STENCIL = 0x40L
};

class RHITexture : public RHIResource
{
public:
    virtual ~RHITexture() = default;

protected:
    RHITexture(
        void* pData,
        int iWidth,
        int iHeight,
        ETextureFormat eFormat
        )
    : RHIResource(pData, ERHICPUAccessFlags::NONE, ERHIBufferUsage::DEFAULT)
    , m_iWidth(iWidth)
    , m_iHeight(iHeight)
    , m_eFormat(eFormat)
    {}

    int             m_iWidth;
    int             m_iHeight;
    ETextureFormat  m_eFormat;
};