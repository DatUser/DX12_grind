#pragma once

#include "RHI/rhi_resource.h"

enum class ETextureFormat : uint8_t
{
    R8G8B8A8_UNORM,
    R8G8B8_UNORM,
    R32G32B32_FLOAT,
    R8_UNORM
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
    : RHIResource(pData, ERHIBufferFlags::CONSTANT, ERHICPUAccessFlags::NONE, ERHIBufferUsage::DEFAULT)
    , m_iWidth(iWidth)
    , m_iHeight(iHeight)
    , m_eFormat(eFormat)
    {}

    int             m_iWidth;
    int             m_iHeight;
    ETextureFormat  m_eFormat;
};