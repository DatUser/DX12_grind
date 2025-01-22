#pragma once

#include "RHI/rhi_buffer.h"

#include <memory>

class RHIBuffer;


/**
 *  This is the rendering interface
 *  Its main purpose is to request frames
 *  and execute command lists
*/
class RHI
{
public:
    virtual ~RHI() = default;


    /**
     *  Present frame to user
    */
    virtual void PresentFrame() = 0;

    virtual void ClearRenderView() = 0;
    virtual void Draw() = 0;

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(const void* pData, unsigned int uByteWidth, ERHIBufferFlags eFlags, ECPUAccessFlags eCPUAccess=ECPUAccessFlags::NONE) = 0;
    virtual void CreateSwapchain() = 0;

	inline static auto&& GetInterface()
	{
		return m_spGFXInterface;
		//return m_spGFXInterface.get();
	}
protected:
	static std::unique_ptr<RHI>	m_spGFXInterface;
};