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

    virtual std::shared_ptr<RHIBuffer> CreateBuffer(void* pData, unsigned int uByteWidth, ERHIBufferFlags eFlags, ECPUAccessFlags eCPUAccess=ECPUAccessFlags::NONE) = 0;
    virtual bool UploadBuffer(const std::shared_ptr<RHIBuffer>& spBuffer) = 0;

    virtual void CreateSwapchain() = 0;

	virtual void SetVertexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetIndexBuffer(const RHIBuffer* pBuffer) = 0;
	virtual void SetBuffer(const RHIBuffer* pBuffer) = 0;

	//inline static auto&& GetInterface()
	inline static RHI* GetInterface()
	{
		//return m_spGFXInterface;
		return m_spGFXInterface.get();
	}
protected:
	static std::unique_ptr<RHI>	m_spGFXInterface;
};