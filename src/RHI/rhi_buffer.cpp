#include "RHI/rhi_buffer.h"

#include <array>
#include <map>

RHIBuffer::RHIBuffer(
	void* pData,
	uint32_t uByteWidth,
	ERHIBufferFlags eFlags,
	ECPUAccessFlags eCPUAccess)
: m_pData(pData)
, m_uByteWidth(uByteWidth)
, m_eFlags(eFlags)
, m_eCPUAccess(eCPUAccess)
{
}
