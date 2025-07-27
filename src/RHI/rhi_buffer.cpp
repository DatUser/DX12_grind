#include "RHI/rhi_buffer.h"

#include <array>
#include <map>

RHIBuffer::RHIBuffer(
	void* pData,
	uint32_t uByteWidth,
	ERHIBufferFlags eFlags,
	ERHICPUAccessFlags eCPUAccess,
	ERHIBufferUsage eUsage
)
: RHIResource(pData, uByteWidth,eCPUAccess, eUsage)
, m_uByteWidth(uByteWidth)
, m_eFlags(eFlags)
{
}