#pragma once

#include <stdint.h>

#include "RHI/rhi_resource.h"

enum class ERHIBufferFlags : uint8_t
{
	CONSTANT,
	INDEX,
	VERTEX,

	_size
};

class RHIBuffer : public RHIResource
{
	friend class D3D11Interface;
public:

	virtual ~RHIBuffer() = default;

	virtual void Update() = 0;
	virtual bool IsValid() = 0;


protected:
	RHIBuffer(
		void* pData,
		uint32_t uByteWidth,
		ERHIBufferFlags eFlags,
		ERHICPUAccessFlags eCPUAccess,
		ERHIBufferUsage eUsage);

	uint32_t 			m_uByteWidth;	// Size of the buffer (bytes)
	uint32_t 			m_uOffset;		// Size of a single element
	uint32_t 			m_uStride;		// Size between each element

	ERHIBufferFlags		m_eFlags;
};