#pragma once

#include <memory>

class RHITexture;

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
    // Texture is empty and only holds a View to backbuffer
    std::shared_ptr<RHITexture> m_spBackBufferRTV;
};