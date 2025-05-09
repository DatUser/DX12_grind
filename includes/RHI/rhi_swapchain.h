#pragma once

#include "RHI/rhi_texture.h"

class RHISwapchain
{
public:
    RHISwapchain() = default;
    virtual ~RHISwapchain() = default;

    virtual void Present() = 0;

    RHITexture* GetBackBufferRTV() const
    {
        return m_spBackBufferRTV.get();
    }

protected:
    std::shared_ptr<RHITexture> m_spBackBufferRTV;
};