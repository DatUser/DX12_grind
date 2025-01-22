#include "RHI/rhi_buffer.h"

#include <array>
#include <map>

RHIBuffer::RHIBuffer(ERHIBufferFlags eFlags, ECPUAccessFlags eCPUAccess)
: m_eFlags(eFlags)
, m_eCPUAccess(eCPUAccess)
{
}
