#pragma once

#include <stdint.h>

#include "fwd_rhi.h"
#include "RHI/rhi_resource.h"

enum class ETextureFormat : uint8_t
{
	// Integer formats
	R8G8B8A8_UNORM,
	R8_UNORM,
	D24_UNORM_S8_UINT,

	// Float formats
	R32G32B32_FLOAT,
	R32G32B32A32_FLOAT,
	R32_FLOAT,

	_size
};

enum class ERHITextureFlags : uint32_t
{
	SHADER_RESOURCE =   1,
	RENDER_TARGET =	 1 << 1,
	DEPTH_STENCIL =	 1 << 2,
	UNORDERED_ACCESS =  1 << 3,

	_size = 4
};

#include "rhi_texture.hpp"

class RHITexture : public RHIResource
{
	friend class D3D11Interface;
public:
	virtual ~RHITexture() = default;

	RHITexture() = default;

	void SetWidth(int iWidth)
	{
		m_iWidth = iWidth;
	}

	void SetHeight(int iHeight)
	{
		m_iHeight = iHeight;
	}

	ETextureFormat GetFormat() const
	{
		return m_eFormat;
	}
protected:

	RHITexture(
		void* pData,
		int iWidth,
		int iHeight,
		ETextureFormat eFormat,
		uint32_t uFlags
		)
	: RHIResource(pData, GetByteSize(eFormat, iWidth, iHeight),ERHICPUAccessFlags::NONE, ERHIBufferUsage::DEFAULT)
	, m_iWidth(iWidth)
	, m_iHeight(iHeight)
	, m_uFlags(uFlags)
	, m_eFormat(eFormat)
	{}

	int			 m_iWidth;
	int			 m_iHeight;
	uint32_t		m_uFlags;
	ETextureFormat  m_eFormat;
};