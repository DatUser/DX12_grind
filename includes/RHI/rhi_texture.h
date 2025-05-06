#pragma once

#include <stdint.h>

#include "fwd_rhi.h"
#include "RHI/rhi_resource.h"

enum class ETextureFormat : uint8_t
{
    R8G8B8A8_UNORM,
    R32G32B32_FLOAT,
    R8_UNORM,

    _size
};

enum class ERHITextureFlags : uint32_t
{
    SHADER_RESOURCE =   1,
    RENDER_TARGET =     1 << 1,
    DEPTH_STENCIL =     1 << 2
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
        ETextureFormat eFormat,
        uint32_t uFlags
        )
    : RHIResource(pData, ERHICPUAccessFlags::NONE, ERHIBufferUsage::DEFAULT)
    , m_iWidth(iWidth)
    , m_iHeight(iHeight)
    , m_eFormat(eFormat)
    , m_uFlags(uFlags)
    {}

    int             m_iWidth;
    int             m_iHeight;
    uint32_t        m_uFlags;
    ETextureFormat  m_eFormat;
};
