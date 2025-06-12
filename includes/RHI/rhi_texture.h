#pragma once

#include <stdint.h>

#include "fwd_rhi.h"
#include "RHI/rhi_resource.h"

enum class ETextureFormat : uint8_t
{
    R8G8B8A8_UNORM,
    R32G32B32_FLOAT,
    R8_UNORM,
    D24_UNORM_S8_UINT,

    _size
};

enum class ERHITextureFlags : uint32_t
{
    SHADER_RESOURCE =   1,
    RENDER_TARGET =     1 << 1,
    DEPTH_STENCIL =     1 << 2,
    UNORDERED_ACCESS =  1 << 3,
};

class RHITexture : public RHIResource
{
    friend class D3D11Interface;
public:
    virtual ~RHITexture() = default;

    RHITexture() = default;
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
    , m_uFlags(uFlags)
    , m_eFormat(eFormat)
    {}

    int             m_iWidth;
    int             m_iHeight;
    uint32_t        m_uFlags;
    ETextureFormat  m_eFormat;
};

constexpr uint32_t operator&(uint32_t uFlags, ERHITextureFlags eFlags)
{
    return uFlags & static_cast<uint32_t>(eFlags);
}

constexpr uint32_t operator|=(uint32_t& uFlags, ERHITextureFlags eFlag)
{
    return uFlags |= static_cast<uint32_t>(eFlag);
}

constexpr uint32_t operator|(ERHITextureFlags eFlagA, ERHITextureFlags eFlagB)
{
    return static_cast<uint32_t>(eFlagA) | static_cast<uint32_t>(eFlagB);
}