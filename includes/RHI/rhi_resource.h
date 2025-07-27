#pragma once

#include <vector>

enum class ERHICPUAccessFlags : uint8_t
{
	NONE,
	READ,
	WRITE,
	READ_WRITE,

	_size
};

enum class ERHIBufferUsage : uint8_t
{
	DEFAULT,
	DYNAMIC,

	_size
};

class RHIResource
{
public:
	std::vector<std::byte>& GetData()
	{
		return m_pData;
	}

protected:
	RHIResource() = default;

	RHIResource(
		void* pData,
		uint32_t uByteWidth,
		ERHICPUAccessFlags eCPUAccess,
		ERHIBufferUsage eUsage
	)
	: m_eCPUAccess(eCPUAccess)
	, m_eUsage(eUsage)
	{
		if (pData == nullptr || uByteWidth == 0)
			return;

		m_pData.resize(uByteWidth);
		std::memcpy(m_pData.data(), pData, uByteWidth);
	}

	virtual ~RHIResource() = default;

	std::vector<std::byte>		m_pData;
	ERHICPUAccessFlags			m_eCPUAccess;
	ERHIBufferUsage				m_eUsage;
};